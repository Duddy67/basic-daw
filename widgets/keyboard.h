#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <string.h>
#include "../constants.h"
#include "../project/controller.h"
#include "../core/pitch_converter.h"


namespace project {
    class Controller;
    class View;
}

class Keyboard : public Fl_Widget
{
    project::Controller& controller;
    project::View& view;
    int hoveredPitch = -1;
    int playingPitch = -1;
    int velocityAtX(int eventX) const;

  protected: 

      void draw() override;
      int handle(int event) override;

  public:

      Keyboard(int x, int y, int w, int h, project::Controller& ctrl, project::View& v);
      ~Keyboard() {}
};

#endif // KEYBOARD_H



