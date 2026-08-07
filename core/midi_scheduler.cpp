#include "midi_scheduler.h"
#include "../midi/track.h"


MidiScheduler::MidiScheduler(Application& app) : application(app)
{
}

MidiScheduler::~MidiScheduler()
{
    // ...
}

void MidiScheduler::process(uint64_t sample)
{
    auto& tracks = application.getProject()->getTracks();

    for (auto& track : tracks) {
        if (track->getType() == DataType::MIDI) {
            auto* midiTrack = static_cast<Midi::Track*>(track.get());

            auto& events = midiTrack->getEvents();

            while (midiTrack->playbackCursor < events.size()) {
                auto& event = events[midiTrack->playbackCursor];

                if (event.samplePosition > sample) {
                    break;
                }

                if (event.samplePosition <= sample) {
                    application.getMidiEngine().getMidiOut().sendMessage(&event.message);
                }

                midiTrack->playbackCursor++;
            }
        }
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
