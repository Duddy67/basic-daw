#ifndef PROJECT_MODEL_H
#define PROJECT_MODEL_H

#include "../main.h"

class Application;

namespace Project {

    class Model {

      Application& application;

      public:

          Model(Application& app);
          ~Model();

          void addTrack();

    };
}

#endif // PROJECT_MODEL_H
