#include "track.h"

namespace Widget {
    Track::Track(int x, int y, int w, int h) : Fl_Group(x, y, w, h)
    {
        box(FL_UP_BOX);
    }

    Track::~Track()
    {
       // Destructor implementation (can be empty).
       // Prevent errors such as: undefined reference to `vtable for WidgetTrack'
    }
}

