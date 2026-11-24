#ifndef RULER_H
#define RULER_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../project/controller.h"
#include "../core/time_converter.h"
#include "../constants.h"

namespace project {
    class Controller;
    class View;
    class Observer;
}

class TempoMap;

class Ruler : public Fl_Group
{
        project::Controller& controller;
        project::View& view;
        TempoMap& tempoMap;

    protected: 
        void draw() override;
        int handle(int event) override;

    public:

        Ruler(int x, int y, int w, int h, project::Controller& ctrl, project::View& v, TempoMap& tm);
        ~Ruler() {}

};

#endif // RULER_H
