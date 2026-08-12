#ifndef MIDI_SCHEDULER_H
#define MIDI_SCHEDULER_H

#include <cstdint>
#include "../main.h"


class Application;

class MidiScheduler {
    
    Application& application;

    public:

        MidiScheduler(Application& app);
        ~MidiScheduler();

        void processOutput(jack_nframes_t nframes, void* midiOutBuffer, uint64_t currentSample);
        void processInput(void* midiInBuffer, uint64_t currentSample);
        void relocate(uint64_t sample);
};

#endif // MIDI_SCHEDULER_H
