#ifndef WIDGET_TRACK_H
#define WIDGET_TRACK_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Light_Button.H>
#include <iostream>
#include "../constants.h"

namespace Project {
    class Controller;
}

namespace Widget {

    class Track : public Fl_Group
    {
        int id;
        DataType type;
        Project::Controller& projectCtrl;
        Fl_Output* name = nullptr;
        Fl_Light_Button* solo = nullptr;
        Fl_Light_Button* mute = nullptr;
        Fl_Light_Button* record = nullptr;
        bool selected = false;

        protected:

            int handle(int event) override;

        public:

            Track(int x, int y, int w, int h, int i, DataType t, Project::Controller& ctrl);
            ~Track();

            int getId() { return id; }
            DataType getType() { return type; }
            bool isSelected() { return selected; }
            void select() { selected = true; }
            void unselect() { selected = false; }
    };
}

#endif // WIDGET_TRACK_H


