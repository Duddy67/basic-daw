#ifndef RULER_H
#define RULER_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../project/controller.h"
#include "../core/time_converter.h"

namespace Project {
    class Controller;
    class Observer;
}

class Ruler : public Fl_Group
{
        Project::Controller& projectCtrl;
        // Pointer to share state with other GUI elements.
        ViewState* viewState;

    protected: 
        void draw() override;
        int handle(int event) override;

    public:

        Ruler(int x, int y, int w, int h, Project::Controller& ctrl);
        ~Ruler() {}

};

#endif // RULER_H
