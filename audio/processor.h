#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

#include <vector>
#include <atomic>
#include "../main.h"
#include "miniaudio.h"

class Application;

namespace Audio {

    class Processor {

        Application& application;

      public:

          Processor(Application& app);
          ~Processor();

          // Called from Jack callback
          void process(jack_nframes_t nframes, float* outLeft, float* outRight, const float* inLeft, const float* inRight);

          void createTrack();
          void removeTrack();
    };
}

#endif // AUDIO_PROCESSOR_H

