#include "grid_bar.h"
#include "../main.h"



GridBar::GridBar(int x, int y, int w, int h, Application& app) :
  Fl_Group(x, y, w, h), application(app)
{
    box(FL_DOWN_BOX);
    snap = new Fl_Light_Button(XLARGE_SPACE + SMALL_SPACE + (TINY_SPACE * 2), MENU_BAR_HEIGHT + TINY_SPACE, BUTTON_WIDTH, BUTTON_HEIGHT / 2, "Snap");
    snap->selection_color(FL_GREEN);

    // Assign a callback function to snap button.
    snap->callback([](Fl_Widget* w, void* userData) {
                          GridBar* gb = static_cast<GridBar*>(userData);
                          gb->onSnap();
                      }, (void*) this);

    // Disable keyboard focus on snap button.
    snap->clear_visible_focus();
}


GridBar::~GridBar()
{

}

void GridBar::onSnap()
{
    snapToGrid = snapToGrid ? false : true;
}
