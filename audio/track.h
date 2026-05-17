#ifndef AUDIO_TRACK_H
#define AUDIO_TRACK_H

#include "engine.h"


namespace Audio {

    class Engine;

    class Track {
        Engine& engine;
        int id = 0;

        public:

            Track(Engine& e, int id);
            ~Track();

            int getId() { return id; }
    };
}

#endif // AUDIO_TRACK_H


