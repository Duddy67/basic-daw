#ifndef MIDI_TRACK_H
#define MIDI_TRACK_H

#include <vector>
#include "../core/track.h"
#include "clip.h"


namespace Midi {
    // Forward declarations (for classes inside the Midi namespace)
    class Clip;

    class Track : public Core::Track {
        bool omni = false;
        int channel = 0;
        std::vector<MidiEvent> events;
        std::vector<Clip> clips;

        public:

            Track(int id);
            ~Track() override;

            //void processMessage(const std::vector<unsigned char>& message, double deltaTime);
            void addEvent(MidiEvent& event);
            void setChannel(int chan) { channel = (unsigned int)chan < MAX_MIDI_CHANNELS && chan > 0 ? chan : 0; }
            int getChannel() const { return channel; }
            const std::vector<Clip>& getClips() const { return clips; }
            std::vector<Clip>& getClips() { return clips; }
            void toggleOmni() { omni = !omni; }
            bool isOmni() const { return omni; }
            const std::vector<MidiEvent>& getEvents() const { return events; }
            // Override virtual functions.
            DataType getType() const override { return DataType::MIDI; }
            size_t playbackCursor = 0;
           
            void fillEventList(); // FOR TEST PURPOSE ONLY
            void dummyClip(); // FOR TEST PURPOSE ONLY
    };
}

#endif // MIDI_TRACK_H

