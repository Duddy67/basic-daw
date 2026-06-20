#ifndef MIDI_TRACK_H
#define MIDI_TRACK_H

#include <vector>
#include "../core/track.h"
#include "engine.h"


namespace Midi {

    class Engine;

    class Track : public Core::Track {
        Engine& engine;
        bool omni = false;
        int channel = 0;
        std::vector<MidiEvent> events;

        public:

            Track(Engine& e, int id);
            ~Track() override;

            void processMessage(const std::vector<unsigned char>& message, double deltaTime);
            void addEvent(MidiEvent& event);
            void setChannel(int chan) { channel = (unsigned int)chan < MAX_MIDI_CHANNELS && chan > 0 ? chan : 0; }
            int getChannel() const { return channel; }
            void toggleOmni() { omni = !omni; }
            bool isOmni() const { return omni; }
            const std::vector<MidiEvent>& getEvents() const { return events; }
            // Override virtual functions.
            TrackType getType() const override { return TrackType::MIDI; }
            size_t playbackCursor = 0;
           
            void fillEventList(); // FOR TEST PURPOSE ONLY
    };
}

#endif // MIDI_TRACK_H

