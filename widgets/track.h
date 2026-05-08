#ifndef WIDGET_TRACK_H
#define WIDGET_TRACK_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>

namespace Widget {

    class Track : public Fl_Group
    {
      public:

          Track(int x, int y, int w, int h);
          ~Track();
    };
}

#endif // WIDGET_TRACK_H


