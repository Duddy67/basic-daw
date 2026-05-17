#include "strip.h"

Strip::Strip(int x, int y, int w, int h, int i, TrackType t) : Fl_Group(x, y, w, h), id(i), type(t)
{
    box(FL_UP_BOX);
    // Stop adding children (Important!).
    end();
}

Strip::~Strip()
{
   // Destructor implementation (can be empty).
   // Prevent errors such as: undefined reference to `vtable for WidgetTrack'
}

