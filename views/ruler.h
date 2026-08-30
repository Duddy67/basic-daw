#ifndef RULER_H
#define RULER_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>

class Ruler : public Fl_Group
{

    public:

        Ruler(int x, int y, int w, int h) : Fl_Group(x, y, w, h)
        {
            box(FL_FLAT_BOX);
            color((Fl_Color) FL_BLUE);
            end();
        }

        ~Ruler() {}

};

#endif // RULER_H
