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

    // Compute start and end beats according to the view state.
    /*double startBeat = viewState->horizontalOffset / viewState->zoom;
    double endBeat = (viewState->horizontalOffset + w()) / viewState->zoom;
    auto tempoMap = projectCtrl.getTempoMap();

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

    //int barNum = tempoMap.getBarNumber(startBeat);

    // Draw the bar lines.
    for (double beat : bars) {
        int pixel = (int)(beat * viewState->zoom) - viewState->horizontalOffset;
        fl_color(FL_BLACK);
        fl_line(x() + pixel, y(), x() + pixel, y() + h());
        //fl_draw(std::to_string(barNum++), x + 2, h() - 4);
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

        //int lineHeight = isStrongBeat ? 

        fl_line(x() + pixel, y(), x() + pixel, y() + h());

                // Optional: label the beat number (e.g., "2", "3", "4")
        if (isStrongBeat) {
            fl_color(FL_BLACK);
            fl_draw(std::to_string((int)beatsInBar + 1).c_str(), x() + 2, 40 - 2);
        }

        beat += gridStep;
    }*/

    projectCtrl.getView().drawGrid(x(), y(), w(), h());
    projectCtrl.getView().drawCursor(x(), y(), w(), h());

    fl_pop_clip();
}

