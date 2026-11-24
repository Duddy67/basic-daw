#include "piano_roll.h"
#include "../core/engine.h"
#include <FL/fl_draw.H>

PianoRoll::PianoRoll(int x, int y, int w, int h, project::Controller& ctrl, project::View& v, TempoMap& tempoMap) :
                      Fl_Group(x, y, w, h), controller(ctrl), view(v)
{
    box(FL_FLAT_BOX);
    controller.addObserver(this);
    // Create the virtual keyboard on the left edge.
    keyboard = new Keyboard(x, y, KEY_LENGHT, h, ctrl);
    // Save room for the keyboard on the left edge.
    noteCanvas = new NoteCanvas(x + KEY_LENGHT, y, w - KEY_LENGHT, h, ctrl, view, tempoMap);
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
    controller.getView().drawPitchGrid(x() + KEY_LENGHT, y(), w() - KEY_LENGHT, h());
    controller.getView().drawGrid(x() + KEY_LENGHT, y(), w() - KEY_LENGHT, h());
    // Playhead on top.
    controller.getView().drawCursor(x() + KEY_LENGHT, y(), w() - KEY_LENGHT, h());

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
    int rowsShown = h() / view.getViewState().keyHeight;
    int lowest = pitch - rowsShown / 2;

    if (lowest < 0) {
        lowest = 0;
    }

    view.getViewState().pitchVerticalOffset = lowest * view.getViewState().keyHeight;
}
