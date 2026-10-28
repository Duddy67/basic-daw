#ifndef MIDI_SCHEDULER_H
#define MIDI_SCHEDULER_H

#include <cstdint>
#include "../main.h"
#include "../project/model.h"
#include "track.h"


class Application;
namespace Project {
    class Controller;
}

namespace Midi {
    class Track;

    class Scheduler {
        
        Application& application;

        public:

            Scheduler(Application& app);
            ~Scheduler();

            void processOutput(jack_nframes_t nframes, void* midiOutBuffer, uint64_t currentSample);
            void processInput(void* midiInBuffer, uint64_t currentSample);
            void relocate(uint64_t sample);
            void sendAllNotesOff(Track& track);
            void compile(Project::Controller& controller);
    };
}

#endif // MIDI_SCHEDULER_H
