#ifndef STRIP_H
#define STRIP_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>
#include <sstream>
#include <string.h>
#include "../constants.h"


namespace Project {
    class Controller;
}

class Strip : public Fl_Group
{
    int id;
    TrackType type;
    Project::Controller& projectCtrl;
    Fl_Choice* channel = nullptr;
    Fl_Check_Button* omni = nullptr;
    void buildMidi();
    void buildAudio();

    public:

        Strip(int x, int y, int w, int h, int i, TrackType t, Project::Controller& ctrl);
        ~Strip();

        int getId() { return id; }
        TrackType getType() { return type; }
};

#endif // STRIP_H



