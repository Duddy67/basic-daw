#ifndef PROJECT_OBSERVER_H
#define PROJECT_OBSERVER_H

#include "../constants.h"


namespace Project {

    class Observer {
      public:

          virtual void onCtrlEvent(CtrlEvent event, int index) = 0;

          // Virtual destructor for proper cleanup.
          virtual ~Observer() {}

    };
}

#endif // PROJECT_OBSERVER_H
