#include "scheduler.h"
#include "track.h"


namespace Midi {

    Scheduler::Scheduler(Application& app) : application(app)
    {
        // ...
    }

    Scheduler::~Scheduler()
    {
        // ...
    }

    /*
     * Called from core engine.
     */
    void Scheduler::processOutput(jack_nframes_t nframes, void* midiOutBuffer, uint64_t currentSample)
    {
        auto& tracks = application.getProject()->getTracks();

        // Loop through tracks.
        for (auto& track : tracks) {
            // Check for MIDI tracks.
            if (track->getType() == DataType::MIDI) {
                auto* midiTrack = static_cast<Midi::Track*>(track.get());

                // Get track's messages (ie: MidiEvent type).
                auto& events = midiTrack->getEvents();

                while (midiTrack->nextEventId < events.size()) {
                    auto& event = events[midiTrack->nextEventId];

                    // Event is before the current sample. Skip it (it shouldn't happens).
                    if (event.samplePosition < currentSample) {
                        midiTrack->nextEventId++;
                        continue;
                    }

                    // Event is beyond the end of this buffer. Stop processing.
                    if (event.samplePosition >= currentSample + nframes) {
                        break;
                    }

                    // Compute offset within this Jack cycle (0 to nframes - 1).
                    jack_nframes_t offset = static_cast<jack_nframes_t>(event.samplePosition - currentSample);

                    // Write track's message to Jack's MIDI output buffer.
                    int ret = jack_midi_event_write(midiOutBuffer, offset, event.message.data(), event.message.size());
                    //std::cout << "jack_midi_event_write: " << std::endl;

                    if (ret != 0) {
                        // Log error if needed (eg: buffer full).
                    }

                    // Move to next event.
                    midiTrack->nextEventId++;
                }
            }
        }
    }

    /*
     * Called from core engine.
     */
    void Scheduler::processInput(void* midiInBuffer, uint64_t currentSample)
    {
        jack_midi_event_t event;
        jack_nframes_t eventCount = jack_midi_get_event_count(midiInBuffer);

        for (jack_nframes_t i = 0; i < eventCount; ++i) {
            jack_midi_event_get(&event, midiInBuffer, i);

            MidiEvent newEvent;
            newEvent.samplePosition = currentSample + event.time;
            newEvent.message.assign(event.buffer, event.buffer + event.size);
        }

    }

    /*
     * Turns the edited notes in the piano roll into events readable by JACK. 
     */
    void Scheduler::compile(Project::Controller& controller)
    {
        auto& tempoMap = controller.getTempoMap();
        int sampleRate = controller.getEngine().getSampleRate();
        auto& tracks = application.getProject()->getTracks();

        // Loop through tracks.
        for (auto& track : tracks) {
            // Get only MIDI tracks.
            if (track->getType() != DataType::MIDI) {
                continue;
            }

            auto* midiTrack = static_cast<Midi::Track*>(track.get());
            std::vector<MidiEvent> compiled;
            int channel = midiTrack->getChannel();

            // Loop through clips.
            for (const auto& clip : midiTrack->getClips()) {
                // Loop through notes.
                for (const auto& note : clip.getNotes()) {
                    int64_t startTick = clip.getStartTick() + note.startTick;
                    int64_t endTick = clip.getStartTick() + note.endTick();

                    // Create the On and Off events for each note.

                    MidiEvent On;
                    On.samplePosition = tempoMap.ticksToSamples(startTick, sampleRate);
                    On.message = {
                        (unsigned char)(0x90 | (channel & 0x0F)),
                        (unsigned char)note.pitch,
                        (unsigned char)note.velocity
                    };

                    MidiEvent Off;
                    Off.samplePosition = tempoMap.ticksToSamples(endTick, sampleRate);
                    Off.message = {
                        (unsigned char)(0x80 | (channel & 0x0F)),
                        (unsigned char)note.pitch,
                        (unsigned char)0x40
                    };

                    // Store the new events.
                    compiled.push_back(std::move(On));
                    compiled.push_back(std::move(Off));
                }
            }

            // Sort the newly compiled events.
            std::stable_sort(compiled.begin(), compiled.end(), 
                [](const MidiEvent& noteA, const MidiEvent& noteB) {
                    if (noteA.samplePosition != noteB.samplePosition) {
                        return noteA.samplePosition < noteB.samplePosition;
                    }

                    // At equal sample position: note off before note on.
                    // Otherwise a legato repeat of the same pitch is killed.
                    bool noteAOff = (noteA.message[0] & 0xF0) == 0x80;
                    bool noteBOff = (noteB.message[0] & 0xF0) == 0x80;
                    return noteAOff && !noteBOff;
                });

            // Add the newly created events to the event vector.
            midiTrack->setEvents(std::move(compiled));
            // Reset the event index to zero as it's the only safe value after a swap.
            midiTrack->nextEventId = 0;
            // Inform other elements recompiling is done.
            controller.doneRecompiling();
        }

        // Inform the controller that the recompiling is done.
        controller.doneRecompiling();
    }

    void Scheduler::relocate(uint64_t sample)
    {
        // Don't relocate while rolling.
        if (application.getTransport().isRolling()) {
            return;
        }

        auto& tracks = application.getProject()->getTracks();

        for (auto& track : tracks) {
            if (track->getType() == DataType::MIDI) {
                auto* midiTrack = static_cast<Midi::Track*>(track.get());
                // Kill whatever the synth is currently holding on this channel.
                sendAllNotesOff(*midiTrack);

                // Reposition the playback cursor. 
                auto& events = midiTrack->getEvents();

                auto it = std::lower_bound(events.begin(), events.end(), sample,
                                            [](const MidiEvent& event, uint64_t sample)
                                            { 
                                                return event.samplePosition < sample;
                                            });

                midiTrack->nextEventId = std::distance(events.begin(), it);
            }
        }
    }

    void Scheduler::sendAllNotesOff(Track& track)
    {
        const unsigned char channel = track.getChannel() & 0x0F;

        // CC 123 - All notes off
        {
            QueuedMidiEvent event;
            event.size = 3;
            event.bytes[0] = 0xB0 | channel;
            event.bytes[1] = 123;
            event.bytes[2] = 0;
            event.samplePosition = 0;
            // Use the queue not the direct writting as the function is called from the UI.
            application.getCoreEngine().pushUiMidiEvent(event);
        }

        // CC 120 - All sounds off
        {
            QueuedMidiEvent event;
            event.size = 3;
            event.bytes[0] = 0xB0 | channel;
            event.bytes[1] = 120;
            event.bytes[2] = 0;
            event.samplePosition = 0;
            application.getCoreEngine().pushUiMidiEvent(event);
        }
    }
}
