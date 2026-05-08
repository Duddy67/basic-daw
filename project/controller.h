#ifndef PROJECT_CONTROLLER_H
#define PROJECT_CONTROLLER_H

#include <vector>
#include <algorithm>
#include "../main.h"
#include "model.h"
#include "observer.h"

class Application;

namespace Project {

    class Model;
    class Observer;

    class Controller {

      Application& pApplication;
      Model& model;
      // List of all registered observers.
      std::vector<Observer*> observers;  

      void notify(ProjectEvent event, int index);

      public:

          Controller(Application& app, Model& m);
          ~Controller();

          // Register an observer.
          void addObserver(Observer* observer) { observers.push_back(observer); }
          void removeObserver(Observer* observer);

          void onAddTrack();
    };
}

#endif // PROJECT_CONTROLLER_H
