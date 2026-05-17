#ifndef STRIP_H
#define STRIP_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../constants.h"


class Strip : public Fl_Group
{
    int id;
    TrackType type;

    public:

        Strip(int x, int y, int w, int h, int i, TrackType t);
        ~Strip();

        int getId() { return id; }
        TrackType getType() { return type; }
};

#endif // STRIP_H



