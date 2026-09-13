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

        // Shortcuts for timeline coordinates.
        int timelineX = x + screenFourth + BORDER_INTERSTICE;
        int timelineY = y + RULER_HEIGHT + BORDER_INTERSTICE;
        int timelineW = (screenFourth * 3) - (BORDER_INTERSTICE * 2) - SCROLLBAR_HEIGHT;
        int timelineH = h - (BORDER_INTERSTICE * 2) - (RULER_HEIGHT + SCROLLBAR_HEIGHT);

        Fl_Group* workspace = new Fl_Group(x + screenFourth / 2, y, Fl::w() - (screenFourth / 2), h);
            ruler = new Ruler(x + screenFourth + BORDER_INTERSTICE, y, (screenFourth * 3) - (BORDER_INTERSTICE * 2), RULER_HEIGHT, controller);
            trackList = new TrackList(x + screenFourth / 2, y + RULER_HEIGHT, screenFourth / 2, h - RULER_HEIGHT, controller);
            timeline = new Timeline(timelineX, timelineY, timelineW, timelineH, controller);

            hScrollbar = new Fl_Scrollbar(timelineX, timelineY + timelineH, timelineW, SCROLLBAR_HEIGHT);
            hScrollbar->type(FL_HORIZONTAL);
            hScrollbar->callback(hScrollbar_cb, this);

            vScrollbar = new Fl_Scrollbar(timelineX + timelineW, timelineY, SCROLLBAR_HEIGHT, timelineH);
            vScrollbar->type(FL_VERTICAL);
            vScrollbar->callback(vScrollbar_cb, this);

            workspace->add(trackList);
            workspace->add(ruler);
            workspace->add(timeline);
            workspace->add(hScrollbar);
            workspace->add(vScrollbar);
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

    Timeline& View::getTimeline()
    {
        return *timeline;
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

    // Functions common to the Ruler and Timeline widgets.

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

        // Draw the bar lines.
        for (double beat : bars) {
            int pixel = (int)(beat * viewState->zoom) - viewState->horizontalOffset;
            fl_color(FL_BLACK);
            fl_line(x + pixel, y, x + pixel, y + h);

            if (isRuler) {
                int barNum = tempoMap.getBarNumber(beat);
                // Draw bar numbers beside the bar lines on the left.
                fl_draw(std::to_string(barNum).c_str(), x + pixel + 2, y + 12);
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

    void View::vScrollbar_cb(Fl_Widget* w, void* data)
    {
        View* self = static_cast<View*>(data);
        self->viewState->verticalOffset = (int)self->vScrollbar->value();
        self->ruler->redraw();
        self->timeline->redraw();
    }

    void View::hScrollbar_cb(Fl_Widget* w, void* data)
    {
        View* self = static_cast<View*>(data);
        self->viewState->horizontalOffset = (int)self->hScrollbar->value();
        self->ruler->redraw();
        self->timeline->redraw();
    }

    void View::updateScrollbars()
    {
        double maxBeats = 10000.0;
        int totalPixels = (int)(maxBeats * viewState->zoom);
        int visiblePixels = timeline->h();

        if (totalPixels > visiblePixels) {
            hScrollbar->range(0, totalPixels - visiblePixels);
            hScrollbar->slider_size((double)visiblePixels / totalPixels);
            hScrollbar->activate();
        }
        else {
            hScrollbar->range(0, 0);
            hScrollbar->slider_size(1.0);
            hScrollbar->deactivate();
        }

    }

    void View::setZoom(double newZoom, int anchorScreenX)
    {
        // Clamp zoom to reasonable limits.
        if (newZoom < MIN_ZOOM) {
            newZoom = MIN_ZOOM;
        }

        if (newZoom > MAX_ZOOM) {
            newZoom = MAX_ZOOM;
        }

        // Find the beat under the anchor point (before zoom).
        double anchorBeat = (viewState->horizontalOffset + anchorScreenX) / viewState->zoom;

        // Apply the new zoom.
        viewState->zoom = newZoom;

        // Recompute the offset so the anchor beat stays at the same screen x.
        viewState->horizontalOffset = (int)((anchorBeat * viewState->zoom) - anchorScreenX);

        if (viewState->horizontalOffset < 0) {
            viewState->horizontalOffset = 0;
        }

        // Update the horizontal scrollbar range and value.
        updateScrollbars();
        // Recompute range based on new zoom.
        hScrollbar->value(viewState->horizontalOffset);

        // Redraw all affected widgets.
        ruler->redraw();
        timeline->redraw();
    }

    /*
     * Handles the events happening into the track widget.
     */
    int View::handle(int event)
    {
        switch (event) {
            case FL_SHORTCUT: {

                // Check for minus sign key.
                if (Fl::event_key() == 54) {
                    zoomOut();
                }

                // Check for plus sign key.
                if (Fl::event_key() == 61) {
                    zoomIn();
                }

                // Event handled - Stop propagation.
                return 1;
            }

            // Right click or other buttons not handled. Let parent widgets see it too.
            return 0;
        }

        // Default - Let Fl_Group handle any not processed events.
        return Fl_Group::handle(event);
    }

    void View::zoomIn()
    {
        // Anchor at the playhead if visible, otherwise at the center.
        int anchorX = getAnchorScreenX();
        setZoom(viewState->zoom * 1.25, anchorX);
    }

    void View::zoomOut()
    {
        int anchorX = getAnchorScreenX();
        setZoom(viewState->zoom / 1.25, anchorX);
    }

    /*
     * Returns a good anchor point: playhead if visible, the widget's center otherwise.
     */
    int View::getAnchorScreenX()
    {
        double playheadBeat = TimeConverter::getCurrentBeat(controller.getTransport(),
                                                            controller.getTempoMap(),
                                                            controller.getEngine().getSampleRate());

        int playheadX = (int)((playheadBeat * viewState->zoom) - viewState->horizontalOffset);

        if (playheadX >= 0 && playheadX < timeline->w()) {
            return playheadX;
        }

        return timeline->w() / 2;
    }
}
