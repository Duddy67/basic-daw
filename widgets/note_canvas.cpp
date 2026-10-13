#include "note_canvas.h"
#include "../midi/track.h"


NoteCanvas::NoteCanvas(int x, int y, int w, int h, Project::Controller& ctrl)
    : Fl_Widget(x, y, w, h), projectCtrl(ctrl)
{
    viewState = &projectCtrl.getViewState();
}

void NoteCanvas::draw()
{
    // Prevents drawing outside the widget boundaries.
    fl_push_clip(x(), y(), w(), h());

    // Clear the background (erases old playhead, grid...).
    fl_color((Fl_Color)  FL_LIGHT1);
    fl_rectf(x(), y(), w(), h());

    // Start with the pitch grid, so grid's vertical lines will be drawn over.
    projectCtrl.getView().drawPitchGrid(x(), y(), w(), h());
    projectCtrl.getView().drawGrid(x(), y(), w(), h());
    projectCtrl.getView().drawNotes(x(), y(), w(), h(), currentClip());
    // Playhead on top.
    projectCtrl.getView().drawCursor(x(), y(), w(), h());

    // Paints PianoRoll's children.
    //draw_children();

    fl_pop_clip();
}

int64_t NoteCanvas::snap(int64_t tick)
{
    if (!projectCtrl.isSnapToGrid()) {
        return tick;
    }

    auto& tempoMap = projectCtrl.getTempoMap();
    double gridBeats = projectCtrl.getView().getGridStep();
    int64_t gridTicks = tempoMap.beatsToTick(gridBeats);

    if (gridTicks <= 0) {
        return tick;
    }

    return ((tick + gridTicks / 2) / gridTicks) * gridTicks;
}

int NoteCanvas::handle(int event)
{
    if (event != FL_PUSH) {
        return 0;
    }

    int mx = Fl::event_x() - x();
    int my = Fl::event_y() - y();

    auto* clip = currentClip();

    if (!clip) {
        return 0;
    }

    // Right click - Delete the note under the cursor.
    if (Fl::event_button() == FL_RIGHT_MOUSE) {
        int edge = 0;
        int idx = hitTest(mx, my, &edge);

        if (idx >= 0) {
            clip->getNotes().erase(clip->getNotes().begin() + idx);
            redraw();
        }

        return 1;
    }

    if (Fl::event_button() != FL_LEFT_MOUSE) {
        return 0;
    }

    // Left click on empty space - Create a note.
    int edge = 0;

    if (hitTest(mx, my, &edge) < 0) {
        auto& tempoMap = projectCtrl.getTempoMap();

        int pitch = PitchConverter::pitchAtY(my + y(), y(), h(), *viewState);
        int64_t tick = TimeConverter::tickAtX(mx, tempoMap, *viewState);
        tick = snap(tick);

        if (tick < 0) {
            tick = 0;
        }

        // Default length: one grid step, snapped in ticks.
        double gridBeats = projectCtrl.getView().getGridStep();
        int64_t gridTicks = tempoMap.beatsToTick(gridBeats);

        Note note;
        note.pitch = pitch;
        note.startTick = tick;
        note.lengthTick = std::max<int64_t>(gridTicks, 1);
        note.velocity = 100;
        note.selected = true;

        // Deselect everything else.
        for (auto& other : clip->getNotes()) {
            other.selected = false;
        }

        clip->getNotes().push_back(note);
        redraw();

        return 1;
    }

    // Left click on a note: select it
    for (auto& other : clip->getNotes()) {
        other.selected = false;
    }

    clip->getNotes()[hitTest(mx, my, &edge)].selected = true;
    redraw();

    return 1;
}

// Returns index of the note under the given widget-relative point, or -1.
// Also reports which edge was hit, for later use by move/resize:
//   edge = -1 : left edge  (resize left)
//   edge =  0 : body       (move)
//   edge = +1 : right edge (resize right)
int NoteCanvas::hitTest(int mx, int my, int* edge) const
{
    const auto* clip = const_cast<NoteCanvas*>(this)->currentClip();

    if (!clip) {
        return -1;
    }

    auto& tempoMap = projectCtrl.getTempoMap();
    int pitch = PitchConverter::pitchAtY(my + y(), y(), h(), *viewState);

    // Iterate in reverse so the topmost-drawn note wins ties.
    const auto& notes = clip->getNotes();

    for (int i = (int)notes.size() - 1; i >= 0; --i) {
        const auto& note = notes[i];

        if (note.pitch != pitch) {
            continue;
        }

        int noteX1 = TimeConverter::xOfTick(note.startTick, tempoMap, *viewState);
        int noteX2 = TimeConverter::xOfTick(note.endTick(), tempoMap, *viewState);

        if (mx < noteX1 || mx >= noteX2) {
            continue;
        }

        if (edge) {
            constexpr int kHandlePx = 4;

            if (mx - noteX1 < kHandlePx) {
                *edge = -1;
            }
            else if (noteX2 - mx < kHandlePx) {
                *edge = +1;
            }
            else {
                *edge = 0;
            }
        }

        return i;
    }

    return -1;
}

Midi::Clip* NoteCanvas::currentClip()
{
    // Get the selected widget track first.
    auto& wTrack = projectCtrl.getView().getTrackList().getSelectedTrack();

    // Make sure it's a MIDI track.
    if (wTrack.getType() != DataType::MIDI) {
        return nullptr;
    }

    // Now get the actual midi track from its id.
    auto* track = projectCtrl.getTrack(wTrack.getId());
    auto* mTrack = static_cast<Midi::Track*>(track);

    // Make sure there is one or more clips.
    if (mTrack->getClips().empty()) {
        return nullptr;
    }

    return &mTrack->getClips()[0];
}
