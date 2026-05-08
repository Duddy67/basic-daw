#ifndef AUDIO_TRACK_H
#define AUDIO_TRACK_H

#include "engine.h"


namespace Audio {

    class Engine;

    class Track {
      Engine& engine;

      public:

          Track(Engine& e);
          ~Track();

    };
}

#endif // AUDIO_TRACK_H


