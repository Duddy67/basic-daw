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

int NoteCanvas::handle(int event)
{

    return 0;
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
