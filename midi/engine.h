#ifndef MIDI_ENGINE_H
#define MIDI_ENGINE_H

#include <vector>
#include "../main.h"
#include "RtMidi.h"


class Application;

namespace Midi {

    class Engine {
        Application& application;
        RtMidiIn* midiIn = nullptr;
        RtMidiOut* midiOut = nullptr;

        public:

            Engine(Application& app);
            ~Engine();

            RtMidiIn& getMidiIn() { return *midiIn; }
            RtMidiOut& getMidiOut() { return *midiOut; }
    };
}


#endif // MIDI_ENGINE_H
