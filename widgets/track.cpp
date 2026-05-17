#include "track.h"

namespace Widget {
    Track::Track(int x, int y, int w, int h, int i, TrackType t) : Fl_Group(x, y, w, h), id(i), type(t)
    {
        box(FL_UP_BOX);
        name = new Fl_Output(x + MICRO_SPACE, y + MICRO_SPACE, w - (MICRO_SPACE * 2), MICRO_SPACE * 5);
        add(name);
        end();
    }

    Track::~Track()
    {
       // Destructor implementation (can be empty).
       // Prevent errors such as: undefined reference to `vtable for WidgetTrack'
    }
}

