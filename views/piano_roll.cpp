#include "piano_roll.h"
#include "../core/engine.h"
#include <FL/fl_draw.H>

PianoRoll::PianoRoll(int x, int y, int w, int h, Project::Controller& ctrl) : Fl_Group(x, y, w, h), projectCtrl(ctrl)
{
    box(FL_FLAT_BOX);
    projectCtrl.addObserver(this);
    viewState = &projectCtrl.getViewState();
    // Create the virtual keyboard on the left edge.
    keyboard = new Keyboard(x, y, KEY_LENGHT, h, ctrl);
    // Save room for the keyboard on the left edge.
    noteCanvas = new NoteCanvas(x + KEY_LENGHT, y, w - KEY_LENGHT, h, ctrl);
    add(keyboard);
    end();
}

void PianoRoll::onCtrlEvent(CtrlEvent event, int index)
{
    switch (event) {
        // Cases that are not managed.
        default:

          return;
    }
}

/*void PianoRoll::draw()
{
    // Prevents drawing outside the widget boundaries.
    fl_push_clip(x(), y(), w(), h());

    // Clear the background (erases old playhead, grid...).
    fl_color((Fl_Color)  FL_LIGHT1);
    fl_rectf(x(), y(), w(), h());

    // Start with the pitch grid, so grid's vertical lines will be drawn over.
    // Save room for the keyboard on the left edge.
    projectCtrl.getView().drawPitchGrid(x() + KEY_LENGHT, y(), w() - KEY_LENGHT, h());
    projectCtrl.getView().drawGrid(x() + KEY_LENGHT, y(), w() - KEY_LENGHT, h());
    // Playhead on top.
    projectCtrl.getView().drawCursor(x() + KEY_LENGHT, y(), w() - KEY_LENGHT, h());

    // Paints PianoRoll's children.
    draw_children();

    fl_pop_clip();
}*/

/*
 * Handles the events happening into the track widget.
 */
int PianoRoll::handle(int event)
{

    // Default - Let Fl_Group handle any not processed events.
    return Fl_Group::handle(event);
}

void PianoRoll::centerOnPitch(int pitch)
{
    int rowsShown = h() / viewState->keyHeight;
    int lowest = pitch - rowsShown / 2;

    if (lowest < 0) {
        lowest = 0;
    }

    viewState->pitchVerticalOffset = lowest * viewState->keyHeight;
}
