#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <vector>
#include "../main.h"

class Application;

namespace Audio {

    class Track;

    class Engine {

      Application* pApplication;

      public:

          Engine(Application* app);
          ~Engine();

          void createTrack();
          void removeTrack();
    };
}

#endif // AUDIO_ENGINE_H

