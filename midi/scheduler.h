#ifndef MIDI_SCHEDULER_H
#define MIDI_SCHEDULER_H

#include <cstdint>
#include <jack/midiport.h>
#include "../main.h"
#include "../project/model.h"
#include "../project/tempo_map.h"
#include "track.h"


class Application;
class TempoMap;

namespace project {
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
            void compile(project::Controller& controller, TempoMap& tempoMap);
    };
}

#endif // MIDI_SCHEDULER_H
