#include "ruler.h"
#include <FL/fl_draw.H>

Ruler::Ruler(int x, int y, int w, int h, Project::Controller& ctrl) : Fl_Group(x, y, w, h), projectCtrl(ctrl)
{
    box(FL_FLAT_BOX);
    color((Fl_Color) FL_LIGHT1);
    viewState = &projectCtrl.getViewState();
    end();
}

void Ruler::draw()
{
    // Prevents drawing outside the widget boundaries.
    fl_push_clip(x(), y(), w(), h());

    // Clear the background (erases old playhead, grid...).
    fl_color((Fl_Color)  FL_LIGHT1);
    fl_rectf(x(), y(), w(), h());

    projectCtrl.getView().drawGrid(x(), y(), w(), h(), true);
    projectCtrl.getView().drawCursor(x(), y(), w(), h());

    fl_pop_clip();
}
