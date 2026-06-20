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

        void process(uint64_t sample);
        void relocate(uint64_t sample);
};

#endif // MIDI_SCHEDULER_H
