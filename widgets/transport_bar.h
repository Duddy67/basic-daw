#ifndef TRANSPORT_BAR_H
#define TRANSPORT_BAR_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <iostream>
#include "../constants.h"


class Application;


class TransportBar : public Fl_Group
{
    Application& application;
    Fl_Button* play = nullptr;
    Fl_Button* stop = nullptr;
    Fl_Button* record = nullptr;
    Fl_Light_Button* loop = nullptr;

    protected:

        int handle(int event) override;

    public:

        TransportBar(int x, int y, int w, int h, Application& app);
        ~TransportBar();

        void onPlay();
        void onStop();
        void onRecord();
        void onLoop();
};

#endif // TRANSPORT_BAR_H
