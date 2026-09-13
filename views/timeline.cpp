#include "timeline.h"
#include "../core/engine.h"
#include <FL/fl_draw.H>

Timeline::Timeline(int x, int y, int w, int h, Project::Controller& ctrl) : Fl_Group(x, y, w, h), projectCtrl(ctrl)
{
    box(FL_FLAT_BOX);
    color((Fl_Color) FL_LIGHT1);
    projectCtrl.addObserver(this);
    viewState = &projectCtrl.getViewState();
    end();
}

void Timeline::onCtrlEvent(CtrlEvent event, int index)
{
    switch (event) {
        // Cases that are not managed.
        default:

          return;
    }
}

void Timeline::draw()
{
    // Prevents drawing outside the widget boundaries.
    fl_push_clip(x(), y(), w(), h());

    // Clear the background (erases old playhead, grid...).
    fl_color((Fl_Color)  FL_LIGHT1);
    fl_rectf(x(), y(), w(), h());

    projectCtrl.getView().drawGrid(x(), y(), w(), h());
    projectCtrl.getView().drawCursor(x(), y(), w(), h());

    fl_pop_clip();
}

/*
 * Handles the events happening into the track widget.
 */
int Timeline::handle(int event)
{
    switch (event) {
        case FL_PUSH: {

            if (Fl::event_button() == FL_LEFT_MOUSE) {

            }
        }

        // Right click or other buttons not handled. Let parent widgets see it too.
        return 0;
    }

    // Default - Let Fl_Group handle any not processed events.
    return Fl_Group::handle(event);
}
