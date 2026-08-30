#ifndef PROJECT_CONTROLLER_H
#define PROJECT_CONTROLLER_H

#include <vector>
#include <algorithm>
#include "../main.h"
#include "model.h"
#include "observer.h"

class Application;
class Transport;
class TempoMap;
namespace Core {
    class Engine;
}

namespace Project {

    class Model;
    class Observer;

    class Controller {

      Application& application;
      Model& model;
      // List of all registered observers.
      std::vector<Observer*> observers;  

      void notify(CtrlEvent event, int index);

      public:

          Controller(Application& app, Model& m);
          ~Controller();

          // Register an observer.
          void addObserver(Observer* observer) { observers.push_back(observer); }
          void removeObserver(Observer* observer);

          void onAddTrack(DataType type);
          void onToggleMute(int trackId, DataType type);
          void onToggleSolo(int trackId, DataType type);
          void onToggleArm(int trackId);
          void onTrackSelected(int id);
          Transport& getTransport();
          TempoMap& getTempoMap();
          Core::Engine& getEngine();
          ViewState& getViewState();
          //Application& getApplication() const { return application; }
    };
}

#endif // PROJECT_CONTROLLER_H
