#ifndef PROJECT_H
#define PROJECT_H

#include "model.h"
#include "controller.h"
#include "view.h"
#include "transport.h"
#include "tempo_map.h"
#include "../constants.h"

namespace project {
    class Model;
    class Controller;
    class View;
}

class Application;
class Transport;
class TempoMap;

class Project
{
    Application& application;
    std::unique_ptr<project::Model> model;
    std::unique_ptr<project::Controller> controller;
    std::unique_ptr<project::View> view;
    std::unique_ptr<Transport> transport;
    std::unique_ptr<TempoMap> tempoMap;
    // The single GUI source of truth. Shared with GUI widgets Ruler, Timeline...
    ViewState viewState;

  public:
    Project(Application& app);
    ~Project();

    void init(int x, int y, int w, int h);
    project::Model& getModel() { return *model; }
    project::Controller& getController() { return *controller; }
    project::View& getView() { return *view; }
    Transport& getTransport() { return *transport; }
    TempoMap& getTempoMap() { return *tempoMap; }
    ViewState& getViewState() { return viewState; }

    project::Model* getModelPtr() { return model.get(); }
    project::View* getViewPtr() { return view.get(); }
    bool hasView() const { return view != nullptr; }
};

#endif // PROJECT_H
