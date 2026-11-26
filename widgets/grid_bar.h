#ifndef GRID_BAR_H
#define GRID_BAR_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Light_Button.H>
#include <iostream>
#include "../constants.h"


class Application;


class GridBar : public Fl_Group
{
    Application& application;
    Fl_Light_Button* snap = nullptr;
    // Pointer to share the project state with other GUI elements.
    ViewState* viewState = nullptr;

    //protected:

        //int handle(int event) override;

    public:

        GridBar(int x, int y, int w, int h, Application& app);
        ~GridBar();

        void setViewState(ViewState& vs);
        void refresh();
        void onSnapToGrid();
};

#endif // GRID_BAR_H

