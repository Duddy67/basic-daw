#include "view.h"

namespace Project {

    View::View(int x, int y, int w, int h, Controller& ctrl) :
      Fl_Group(x, y, w, h), controller(ctrl)
    {
        int screenFourth = Fl::w() / 4; 

        channelStrip = new ChannelStrip(x, y, screenFourth / 2, h);
        trackList = new TrackList(x + screenFourth / 2, y, screenFourth / 2, h, controller);
        trackList->box(FL_DOWN_BOX);
        timeline = new Timeline(x + screenFourth, y, screenFourth * 3, h);
        //Fl_Group* bottomRow = new Fl_Group(x, y + h - 100, w, 100);
        //bottomRow->box(FL_DOWN_BOX);
        //bottomRow->color((Fl_Color) FL_INACTIVE_COLOR);
        add(channelStrip);
        add(trackList);
        add(timeline);

        end();

        // Elements shouldn't be resizable.
        resizable(nullptr);
    }

    View::~View()
    {
       // Destructor implementation (can be empty).
       // Prevent errors such as: undefined reference to `vtable for ProjectView'
    }
}
