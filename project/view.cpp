#include "view.h"
#include <FL/fl_draw.H>

namespace Project {

    View::View(int x, int y, int w, int h, Controller& ctrl) :
      Fl_Group(x, y, w, h), controller(ctrl)
    {
        viewState = &controller.getViewState();

        // Based on the width of the current screen (ie: Fl::w()).
        int screenFourth = Fl::w() / 4; 

        // Create the project's elements.

        channelStrip = new ChannelStrip(x, y, screenFourth / 2, h, controller);

        Fl_Group* workspace = new Fl_Group(x + screenFourth / 2, y, Fl::w() - (screenFourth / 2), h);
            ruler = new Ruler(x + screenFourth + BORDER_INTERSTICE, y, (screenFourth * 3) - (BORDER_INTERSTICE * 2), SMALL_SPACE, controller);
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
        self->ruler->redraw();

        if (self->isLiveUpdating) {
            // Keep the timer running.
            Fl::repeat_timeout(0.01, liveUpdate_cb, userData); // 10 ms refresh
        }

    }

    // Function common to the Ruler and Timeline widgets.

    void View::drawCursor(int x, int y, int w, int h)
    {
        double playhead = TimeConverter::getCurrentBeat(controller.getTransport(),
                                                        controller.getTempoMap(),
                                                        controller.getEngine().getSampleRate());

        int pixel = (int)(playhead * viewState->zoom) - viewState->horizontalOffset;

        // Draw the playhead.
        // Important: Make the pixel value relative to the actual direct widget 
        //            parent, not to the main window (ie: add widget's x value to pixel).
        if (pixel + x >= x && pixel < x + w) {
            fl_color(FL_RED);
            fl_line(pixel + x, y, pixel + x, y + h);
        }
    }

    void View::drawGrid(int x, int y, int w, int h, bool isRuler/* = false*/)
    {
        // Compute start and end beats according to the view state.
        double startBeat = viewState->horizontalOffset / viewState->zoom;
        double endBeat = (viewState->horizontalOffset + w) / viewState->zoom;
        auto tempoMap = controller.getTempoMap();

        // Choose grid step (smallest subdivision)
        double gridStep;

        if (viewState->zoom < 20.0) {
            gridStep = 4.0;
        }      
        else if (viewState->zoom < 50.0) {
            gridStep = 2.0;
        } 
        else if (viewState->zoom < 100.0) {
            gridStep = 1.0;
        }
        else if (viewState->zoom < 200.0) {
            gridStep = 0.5;
        }
        else {
            gridStep = 0.25;
        }

        auto bars = tempoMap.getBarLines(startBeat, endBeat);

        int barNum = tempoMap.getBarNumber(startBeat);

        // Draw the bar lines.
        for (double beat : bars) {
            int pixel = (int)(beat * viewState->zoom) - viewState->horizontalOffset;
            fl_color(FL_BLACK);
            fl_line(x + pixel, y, x + pixel, y + h);

            if (isRuler) {
                // Draw bar numbers beside the bar lines on the left.
                fl_draw(std::to_string(barNum++).c_str(), x + pixel + 2, y + 12);
            }
        }

        double beat = std::ceil(startBeat / gridStep) * gridStep;
        const double eps = 1e-9;

        while (beat <= endBeat) {
            int pixel = (int) std::round((beat * viewState->zoom) - viewState->horizontalOffset);

            double barStart = tempoMap.getBarStart(beat);
            auto [barNum, beatsInBar] = tempoMap.getBarBeat(beat);

            // Skip if this beat is exactly on a bar line (already drawn).
            if (std::fabs(beat - barStart) < eps) {
                beat += gridStep;
                continue;
            }

            bool isDownbeat = (std::fabs(beatsInBar) < eps);
            // Set color/style based on the position in bar.
            bool isStrongBeat = (std::fabs(std::round(beatsInBar) - beatsInBar) < eps) && !isDownbeat;
            bool isHalfBeat = (std::fabs(std::round(beatsInBar * 2.0) / 2.0 - beatsInBar) < eps) && !isDownbeat && !isStrongBeat;

            if (isStrongBeat) {
                fl_color(FL_DARK2);
            }
            else if (isHalfBeat) {
                fl_color(FL_DARK1);
            }
            else {
                fl_color(FL_WHITE);
            }

            int hOffset = 0;

            if (isRuler) {
                // Shorten beat lines in the ruler.
                hOffset = (h / 2);
            }

            fl_line(x + pixel, y + hOffset, x + pixel, y + hOffset + h);

            // Optional: label the beat number (e.g., "2", "3", "4")
            if (isRuler && isStrongBeat) {
                fl_color(FL_DARK2);
                fl_draw(std::to_string((int)beatsInBar + 1).c_str(), x + pixel - 2, y + 18);
            }

            beat += gridStep;
        }
    }
}
