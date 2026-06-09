#ifndef MIDI_TRACK_H
#define MIDI_TRACK_H

#include "../core/track.h"
#include "engine.h"


namespace Midi {

    class Engine;

    class Track : public Core::Track {
        Engine& engine;
        bool omni = false;
        int channel = 0;

        public:

            Track(Engine& e, int id);
            ~Track() override;

            void processMessage(const std::vector<unsigned char>& message, double deltaTime) const;
            void setChannel(int chan) { channel = (unsigned int)chan < MAX_MIDI_CHANNELS && chan > 0 ? chan : 0; }
            int getChannel() const { return channel; }
            void toggleOmni() { omni = !omni; }
            bool isOmni() const { return omni; }
            // Override virtual functions.
            TrackType getType() const override { return TrackType::MIDI; }
    };
}

#endif // MIDI_TRACK_H

