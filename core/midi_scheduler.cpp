#include "midi_scheduler.h"
#include "../midi/track.h"


MidiScheduler::MidiScheduler(Application& app) : application(app)
{
    // ...
}

MidiScheduler::~MidiScheduler()
{
    // ...
}

/*
 * Called from core engine.
 */
void MidiScheduler::processOutput(jack_nframes_t nframes, void* midiOutBuffer, uint64_t currentSample)
{
    if (!application.getProject()) {
        return;
    }

    auto& tracks = application.getProject()->getTracks();

    for (auto& track : tracks) {
        if (track->getType() == DataType::MIDI) {
            auto* midiTrack = static_cast<Midi::Track*>(track.get());

            // Get track's messages (ie: MidiEvent type).
            auto& events = midiTrack->getEvents();

            while (midiTrack->playbackCursor < events.size()) {
                auto& event = events[midiTrack->playbackCursor];

                // Event is before the current sample. Skip it (it shouldn't happens).
                if (event.samplePosition < currentSample) {
                    midiTrack->playbackCursor++;
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

                if (ret != 0) {
                    // Log error if needed (eg: buffer full).
                }

                // Move to next event.
                midiTrack->playbackCursor++;
            }
        }
    }
}

/*
 * Called from core engine.
 */
void MidiScheduler::processInput(void* midiInBuffer, uint64_t currentSample)
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

void MidiScheduler::relocate(uint64_t sample)
{
    auto& tracks = application.getProject()->getTracks();

    for (auto& track : tracks) {
        if (track->getType() == DataType::MIDI) {
            auto* midiTrack = static_cast<Midi::Track*>(track.get());
            auto& events = midiTrack->getEvents();

            auto it = std::lower_bound(events.begin(), events.end(), sample,
                                        [](const MidiEvent& event, uint64_t sample)
                                        { 
                                            return event.samplePosition < sample;
                                        });

            midiTrack->playbackCursor = std::distance(events.begin(), it);
        }
    }
}
