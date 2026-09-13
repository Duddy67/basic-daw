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

/*
 * Handles the events happening into the track widget.
 */
int Ruler::handle(int event)
{
    switch (event) {
        case FL_PUSH: {

            // The user has clicked into the ruler.
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                // Move and relocate the playheader.

                // Mouse x relative to the widget.
                int mouseX = Fl::event_x() - x();

                // Convert pixel to beat.
                // The viewport's left edge corresponds to beat: startBeat = horizontalOffset / zoom
                // Adding mouseX / zoom gives the beat at the cursor.
                double beat = (mouseX + viewState->horizontalOffset) / viewState->zoom;

                if (beat < 0.0) {
                    beat = 0.0;
                }

                // Convert beat to sample via ticks.
                auto& tempoMap = projectCtrl.getTempoMap();
                int sampleRate = projectCtrl.getEngine().getSampleRate();
                int64_t tick = tempoMap.beatsToTick(beat);
                uint64_t sample = (uint64_t)tempoMap.ticksToSamples(tick, sampleRate);

                // Relocate the transport.
                projectCtrl.getTransport().locateToSample(sample);

                // Redraw all widgets that show the playheader.
                redraw();
                projectCtrl.getView().getTimeline().redraw();

                // Event handled - Stop propagation.
                return 1;
            }

            return 0;
        }

        // Right click or other buttons not handled. Let parent widgets see it too.
        return 0;
    }

    // Default - Let Fl_Group handle any not processed events.
    return Fl_Group::handle(event);
}
