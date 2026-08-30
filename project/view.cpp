#include "view.h"

namespace Project {

    View::View(int x, int y, int w, int h, Controller& ctrl) :
      Fl_Group(x, y, w, h), controller(ctrl)
    {
        // Based on the width of the current screen (ie: Fl::w()).
        int screenFourth = Fl::w() / 4; 

        // Create the project's elements.

        channelStrip = new ChannelStrip(x, y, screenFourth / 2, h, controller);

        Fl_Group* workspace = new Fl_Group(x + screenFourth / 2, y, Fl::w() - (screenFourth / 2), h);
            ruler = new Ruler(x + screenFourth, y, (screenFourth * 3) - (BORDER_INTERSTICE * 2), SMALL_SPACE);
            trackList = new TrackList(x + screenFourth / 2, y + SMALL_SPACE, screenFourth / 2, h, controller);
            timeline = new Timeline(x + screenFourth + BORDER_INTERSTICE, y + SMALL_SPACE + BORDER_INTERSTICE,
                                    (screenFourth * 3) - (BORDER_INTERSTICE * 2), h - (BORDER_INTERSTICE * 2) - SMALL_SPACE, controller);
            workspace->add(trackList);
            workspace->add(timeline);
        workspace->end();

        add(channelStrip);
        add(workspace);
        end();

        // Elements shouldn't be resizable.
        resizable(nullptr);
    }

    View::~View()
    {
       // Destructor implementation (can be empty).
       // Prevent errors such as: undefined reference to `vtable for ProjectView'
    }

    void View::startLiveUpdate()
    {
        if (isLiveUpdating) {
            return;
        }

        isLiveUpdating = true;

        // Start the GUI timer.
        Fl::add_timeout(0.01, liveUpdate_cb, this); // 10 ms refresh

    }

    void View::stopLiveUpdate()
    {
        isLiveUpdating = false;
        Fl::remove_timeout(liveUpdate_cb, this);
    }

    void View::liveUpdate_cb(void* userData)
    {
        View* self = static_cast<View*>(userData);

        self->timeline->redraw();

        if (self->isLiveUpdating) {
            // Keep the timer running.
            Fl::repeat_timeout(0.01, liveUpdate_cb, userData); // 10 ms refresh
        }

    }
}
