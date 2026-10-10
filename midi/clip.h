#ifndef MIDI_CLIP_H
#define MIDI_CLIP_H

#include <vector>
#include "../constants.h"


namespace Midi {

    class Clip {
        int64_t startTick = 0;
        int64_t lengthTick = 0;
        std::vector<Note> notes;

      public:

        Clip() {}
        ~Clip() {}

        const std::vector<Note>& getNotes() const { return notes; }
        std::vector<Note>& getNotes() { return notes; }
        uint64_t getStartTick() const { return startTick; }
        uint64_t getLengthTick() const { return lengthTick; }
    };
}

#endif // MIDI_CLIP_H

