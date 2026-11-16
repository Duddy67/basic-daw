#include "project.h"


Project::Project(Application& app) : application(app)
{
    // ...
}

void Project::init(int x, int y, int w, int h)
{
    // First make sure the project is not initialized twice.
    if (model) {
        return;
    }

    transport = std::make_unique<Transport>(application);
    tempoMap = std::make_unique<TempoMap>(application);
    model = std::make_unique<project::Model>(application, viewState);
    controller = std::make_unique<project::Controller>(*model);
    view = std::make_unique<project::View>(x, y, w, h, *controller);
    view->init();
}
