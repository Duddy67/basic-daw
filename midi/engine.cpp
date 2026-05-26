#include "engine.h"


namespace Midi {
    Engine::Engine(Application& app) : application(app)
    {
        // Instantiate RtMidi subclasses. Let exceptions propagate.
        midiIn = new RtMidiIn();          
        midiOut = new RtMidiOut();          
    }

    Engine::~Engine()
    {
        delete midiIn;
        delete midiOut;
    }
}

