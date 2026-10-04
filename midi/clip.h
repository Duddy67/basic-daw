#ifndef MIDI_CLIP_H
#define MIDI_CLIP_H

#include <vector>
#include "../core/constants.h"


namespace Midi {

    class Clip {
        int64_t startTick = 0;
        int64_t lengthTick = 0;
        std::vector<Note> notes;

        public:

            Clip() {}
            ~Clip() {}
    };
}

#endif // MIDI_CLIP_H

