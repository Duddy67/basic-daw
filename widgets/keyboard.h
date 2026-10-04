#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <string.h>
#include "../constants.h"
#include "../project/controller.h"
#include "../core/pitch_converter.h"


namespace Project {
    class Controller;
}

class Keyboard : public Fl_Widget
{
    Project::Controller& projectCtrl;
    int hoveredPitch = -1;
    int playingPitch = -1;
    // Pointer to share state with other GUI elements.
    ViewState* viewState;
    int velocityAtX(int eventX) const;

  protected: 

      void draw() override;
      int handle(int event) override;

  public:

      Keyboard(int x, int y, int w, int h, Project::Controller& ctrl);
      ~Keyboard() {}

};

#endif // KEYBOARD_H



