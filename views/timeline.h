#ifndef TIMELINE_H
#define TIMELINE_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../project/observer.h"
#include "../project/controller.h"
#include "../core/time_converter.h"

namespace project {
    class Controller;
    class Observer;
}


class Timeline : public Fl_Group, public project::Observer
{
    project::Controller& projectCtrl;
    // Pointer to share state with other GUI elements.
    ViewState* viewState;

  protected: 
      void draw() override;
      int handle(int event) override;

  public:

      Timeline(int x, int y, int w, int h, project::Controller& ctrl);
      ~Timeline() {}

      void onCtrlEvent(CtrlEvent event, int index);
};

#endif // TIMELINE_H



