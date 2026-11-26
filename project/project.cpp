#include "project.h"


Project::Project(Core::Engine& e, Midi::Scheduler& ms) : engine(e), midiScheduler(ms)
{
    // ...
}

Project::~Project()
{
    // Clear callbacks before members are destroyed, so nothing can invoke them
    // while View is being torn down.
    if (transport) {
        transport->setPlayCallbacks(nullptr, nullptr);
    } 
    // unique_ptr members destroyed in reverse declaration order after this point.
}

void Project::init(int x, int y, int w, int h)
{
    // First make sure the project is not initialized twice.
    if (model) {
        return;
    }

    uint32_t sampleRate = engine.getSampleRate();
    transport = std::make_unique<Transport>();
    tempoMap = std::make_unique<TempoMap>(sampleRate);
    model = std::make_unique<project::Model>();
    controller = std::make_unique<project::Controller>(*model, *transport, *tempoMap, midiScheduler, engine);
    view = std::make_unique<project::View>(x, y, w, h, *model, *controller, *transport, *tempoMap, viewState);

    // Wire the inversion: Transport publishes, View reacts.
    // The only place in the codebase that knows both transport and view exist.
    transport->setPlayCallbacks(
        [this] { view->startLiveUpdate(); }, 
        [this] { view->stopLiveUpdate(); }
    );

    view->init();
}
