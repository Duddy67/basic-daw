#ifndef PIANO_ROLL_H
#define PIANO_ROLL_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../project/observer.h"
#include "../project/controller.h"
#include "../core/time_converter.h"

namespace Project {
    class Controller;
    class Observer;
}


class PianoRoll : public Fl_Group, public Project::Observer
{
    Project::Controller& projectCtrl;
    // Pointer to share state with other GUI elements.
    ViewState* viewState;

  protected: 
      void draw() override;
      int handle(int event) override;

  public:

      PianoRoll(int x, int y, int w, int h, Project::Controller& ctrl);
      ~PianoRoll() {}

      void onCtrlEvent(CtrlEvent event, int index);
};

#endif // PIANO_ROLL_H



