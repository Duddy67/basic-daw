#ifndef MIDI_TRACK_H
#define MIDI_TRACK_H

#include "engine.h"


namespace Midi {

    class Engine;

    class Track {
        Engine& engine;
        int id = 0;
        bool mute = false;
        bool solo = false;
        bool arm = false;
        bool omni = false;
        int channel = 0;

        public:

            Track(Engine& e, int id);
            ~Track();

            int getId() { return id; }
            void processMessage(const std::vector<unsigned char>& message, double deltaTime) const;

            void setChannel(int chan) { channel = (unsigned int)chan < MAX_MIDI_CHANNELS && chan > 0 ? chan : 0; }
            int getChannel() const { return channel; }
            void toggleMute() { mute = !mute; }
            bool isMuted() const { return mute; }
            void toggleSolo() { solo = !solo; }
            bool isSoloed() const { return solo; }
            void toggleOmni() { omni = !omni; }
            bool isOmni() const { return omni; }
    };
}

#endif // MIDI_TRACK_H



