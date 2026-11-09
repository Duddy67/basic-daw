#ifndef CHANNEL_STRIP_H
#define CHANNEL_STRIP_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../project/observer.h"
#include "../project/controller.h"
#include "../widgets/strip.h"

namespace project {
    class Controller;
    class Observer;
}

class Strip;


class ChannelStrip : public Fl_Group, public project::Observer
{
    project::Controller& projectCtrl;
    std::vector<Strip*> strips;
    void showCurrentStrip(int id);

    public:

        ChannelStrip(int x, int y, int w, int h, project::Controller& ctrl);
        ~ChannelStrip() {}

        void onCtrlEvent(CtrlEvent event, int index);
        void addStrip(int id, DataType type);
};

#endif // CHANNEL_STRIP_H



