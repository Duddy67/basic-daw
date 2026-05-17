#ifndef WIDGET_TRACK_H
#define WIDGET_TRACK_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include "../constants.h"

namespace Widget {

    class Track : public Fl_Group
    {
        Fl_Output* name = nullptr;
        int id;
        TrackType type;
        // Make new tracks selected by default.
        bool selected = true;

        public:

            Track(int x, int y, int w, int h, int i, TrackType t);
            ~Track();

            int getId() { return id; }
            TrackType getType() { return type; }
            void select() { selected = true; }
            void unselect() { selected = false; }
    };
}

#endif // WIDGET_TRACK_H


