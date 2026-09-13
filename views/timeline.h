#ifndef TIMELINE_H
#define TIMELINE_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Box.H>
#include "../project/observer.h"
#include "../project/controller.h"
#include "../core/time_converter.h"

namespace Project {
    class Controller;
    class Observer;
}


class Timeline : public Fl_Group, public Project::Observer
{
    Project::Controller& projectCtrl;
    Fl_Scrollbar* vScrollbar = nullptr;
    Fl_Scrollbar* hScrollbar = nullptr;
    // Pointer to share state with other GUI elements.
    ViewState* viewState;

  protected: 
      void draw() override;
      int handle(int event) override;

  public:

      Timeline(int x, int y, int w, int h, Project::Controller& ctrl);
      ~Timeline() {}

      void onCtrlEvent(CtrlEvent event, int index);
};

#endif // TIMELINE_H



