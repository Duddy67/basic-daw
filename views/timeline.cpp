#include "timeline.h"
#include "../core/engine.h"
#include <FL/fl_draw.H>

Timeline::Timeline(int x, int y, int w, int h, Project::Controller& ctrl) : Fl_Group(x, y, w, h), projectCtrl(ctrl)
{
    box(FL_FLAT_BOX);
    color((Fl_Color) FL_DARK1);
    projectCtrl.addObserver(this);
    viewState = &projectCtrl.getViewState();
        std::cout << "x: " << x << " y: " << y << std::endl;
        std::cout << "w: " << w << " h: " << h << std::endl;
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
    fl_color((Fl_Color) FL_DARK1);
    fl_rectf(x(), y(), w(), h());

    double playhead = TimeConverter::getCurrentBeat(projectCtrl.getTransport(),
                                                       projectCtrl.getTempoMap(),
                                                       projectCtrl.getEngine().getSampleRate());

    int pixel = (int)(playhead * viewState->zoom) - viewState->horizontalOffset;
    // Important: Make the pixel value relative to the direct
    //            parent (ie: Timeline), not to the main window.
    pixel = x() + pixel;

    // Draw the playhead.
    if (pixel >= 0 + x() && pixel < w()) {
        fl_color(FL_RED);
        fl_line(pixel, y(), pixel, h());
    }

    fl_pop_clip();
}

