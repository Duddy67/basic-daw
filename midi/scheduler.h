#ifndef MIDI_SCHEDULER_H
#define MIDI_SCHEDULER_H

#include <cstdint>
#include "../main.h"


class Application;

namespace Midi {

    class Scheduler {
        
        Application& application;

        public:

            Scheduler(Application& app);
            ~Scheduler();

            void processOutput(jack_nframes_t nframes, void* midiOutBuffer, uint64_t currentSample);
            void processInput(void* midiInBuffer, uint64_t currentSample);
            void relocate(uint64_t sample);
    };
}

#endif // MIDI_SCHEDULER_H
