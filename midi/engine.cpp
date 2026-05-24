#include "engine.h"

namespace Midi {
    Engine::Engine(Application& app) : application(app)
    {
        // ...
    }

    Engine::~Engine()
    {
        delete midiIn;
        delete midiOut;
    }
}

