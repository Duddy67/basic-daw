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
        project::Controller& projectCtrl;
        // Pointer to share state with other GUI elements.
        ViewState* viewState;

    protected: 
        void draw() override;
        int handle(int event) override;

    public:

        Ruler(int x, int y, int w, int h, project::Controller& ctrl);
        //Ruler(int x, int y, int w, int h, project::Controller& ctrl, project::View&, TempoMap&);
        ~Ruler() {}

};

#endif // RULER_H
