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

int64_t NoteCanvas::minNoteLengthTicks() const
{
    constexpr int kMinNotePixel = 8;
    auto& tempoMap = projectCtrl.getTempoMap();
    double gridBeats = projectCtrl.getView().getGridStep();

    int64_t gridTicks = tempoMap.beatsToTick(gridBeats);
    int64_t pixelTicks = tempoMap.beatsToTick(kMinNotePixel / viewState->zoom);

    return std::max<int64_t>(gridTicks, pixelTicks);
}

int NoteCanvas::handle(int event)
{
    // Important: Make sure the view does exist before going any further.
    // note: FLTK might dispatches an FL_HIDE event while the view is not entirely created. 
    if (!projectCtrl.hasView()) {
        return 0;
    }

    auto* clip = currentClip();

    if (!clip) {
        return 0;
    }

    // Convert once at the top. moveX, moveY are widget-relative.
    int moveX = Fl::event_x() - x();
    int moveY = Fl::event_y() - y();

    switch (event) {
        case FL_PUSH: {
            // Allows the delete key to reach the canvas.
            Fl::focus(this);

            // Right click - Delete the note under the cursor.
            if (Fl::event_button() == FL_RIGHT_MOUSE) {
                int edge = 0;
                int idx = hitTest(moveX, moveY, &edge);

                if (idx >= 0) {
                    clip->getNotes().erase(clip->getNotes().begin() + idx);
                    projectCtrl.notifyNotesChanged();
                    redraw();
                }

                return 1;
            }

            if (Fl::event_button() != FL_LEFT_MOUSE) {
                return 0;
            }

            int edge = 0;
            int idx = hitTest(moveX, moveY, &edge);

            // Left click on an existing note: begin move/resize.
            if (idx >= 0) {
                // Deselect everything
                for (auto& note : clip->getNotes()) {
                    note.selected = false;
                }

                // but the clicked note.
                clip->getNotes()[idx].selected = true;

                dragNoteIdx = idx;
                dragOriginal = clip->getNotes()[idx];
                dragStartX = moveX;
                dragStartY = moveY;

                if (edge < 0) {
                    mode = DragMode::RESIZE_L;
                }
                else if (edge > 0) {
                    mode = DragMode::RESIZE_R;
                }
                else {
                    mode = DragMode::MOVE;
                }

                redraw();
                return 1;
            }

            auto& tempoMap = projectCtrl.getTempoMap();

            int pitch = PitchConverter::pitchAtY(moveY + y(), y(), h(), *viewState);
            int64_t tick = TimeConverter::tickAtX(moveX, tempoMap, *viewState);
            tick = snap(tick);

            if (tick < 0) {
                tick = 0;
            }

            // Create the new note.
            Note note;
            note.pitch = pitch;
            note.startTick = tick;
            note.lengthTick = minNoteLengthTicks();
            note.velocity = 100;
            note.selected = true;

            // Deselect everything else.
            for (auto& other : clip->getNotes()) {
                other.selected = false;
            }

            // Store the newly created note.
            clip->getNotes().push_back(note);

            dragNoteIdx = (int)clip->getNotes().size() - 1;
            dragOriginal = note;
            dragStartX = moveX;
            dragStartY = moveY;
            mode = DragMode::CREATE;
            projectCtrl.notifyNotesChanged();

            redraw();
            return 1;
        }

        case FL_DRAG: {
            if (mode == DragMode::NONE) {
                return 0;
            }

            if (dragNoteIdx < 0) {
                return 0;
            }

            if (dragNoteIdx >= (int)clip->getNotes().size()) {
                return 0;
            }

            auto& tempoMap = projectCtrl.getTempoMap();
            auto& note = clip->getNotes()[dragNoteIdx];

            // Mouse up = pitch up.
            int deltaPitch = (dragStartY - moveY) / viewState->keyHeight;
            // Tick delta relative to where the mouse was at FL_PUSH.
            int64_t deltaTick = TimeConverter::tickAtX(moveX, tempoMap, *viewState) - TimeConverter::tickAtX(dragStartX, tempoMap, *viewState);
            int64_t minLength = minNoteLengthTicks();

            switch (mode) {
                case DragMode::CREATE: {
                    int64_t newEnd = snap(dragOriginal.startTick + deltaTick);

                    if (newEnd - note.startTick < minLength) {
                        newEnd = note.startTick + minLength;
                    }

                    note.lengthTick = newEnd - note.startTick;
                    break;
                }

                case DragMode::MOVE: {
                    note.pitch = std::clamp(dragOriginal.pitch + deltaPitch, 0, 127);
                    int64_t newStart = snap(dragOriginal.startTick+ deltaTick);

                    if (newStart < 0) {
                        newStart = 0;
                    }

                    note.startTick = newStart;
                    break;
                }

                case DragMode::RESIZE_L: {
                    int64_t newStart = snap(dragOriginal.startTick + deltaTick);

                    if (newStart < 0) {
                        newStart = 0;
                    }

                    int64_t origEnd = dragOriginal.endTick();

                    // Enforce minimum length, not just zero-length.
                    if (newStart > origEnd - minLength) {
                        newStart = origEnd - minLength;
                    }

                    if (newStart < 0) {
                        newStart = 0;
                    }

                    note.startTick = newStart;
                    note.lengthTick = origEnd - newStart;
                    break;
                }

                case DragMode::RESIZE_R: {
                    int64_t newEnd = snap(dragOriginal.endTick() + deltaTick);

                    if (newEnd < note.startTick + minLength) {
                        newEnd = note.startTick + minLength;
                    }

                    note.lengthTick = newEnd - note.startTick;
                    break;
                }

                default:
                    break;
            }

            redraw();
            return 1;
        }

        case FL_RELEASE: {
            if (mode == DragMode::NONE) {
                return 0;
            }

            mode = DragMode::NONE; 
            dragNoteIdx = -1;

            projectCtrl.notifyNotesChanged();

            redraw();
            return 1;
        }
                          
        case FL_KEYBOARD:
            // Check for the delete key.
            if (Fl::event_key() == FL_Delete) {
                // Delete the selected note(s).
                auto& notes = clip->getNotes();
                notes.erase(std::remove_if(notes.begin(), notes.end(), [](const Note& note){ return note.selected; }), notes.end());
                projectCtrl.notifyNotesChanged();
                redraw();
                return 1;
            }

            return 0;

        default:
            return Fl_Widget::handle(event);
    }
}

// Returns index of the note under the given widget-relative point, or -1.
// Also reports which edge was hit, for later use by move/resize:
//   edge = -1 : left edge  (resize left)
//   edge =  0 : body       (move)
//   edge = +1 : right edge (resize right)
int NoteCanvas::hitTest(int moveX, int moveY, int* edge) const
{
    const auto* clip = const_cast<NoteCanvas*>(this)->currentClip();

    if (!clip) {
        return -1;
    }

    auto& tempoMap = projectCtrl.getTempoMap();
    int pitch = PitchConverter::pitchAtY(moveY + y(), y(), h(), *viewState);

    const auto& notes = clip->getNotes();

    // Iterate in reverse so the topmost-drawn note wins ties.
    for (int i = (int)notes.size() - 1; i >= 0; --i) {
        const auto& note = notes[i];

        if (note.pitch != pitch) {
            continue;
        }

        int noteX1 = TimeConverter::xOfTick(note.startTick, tempoMap, *viewState);
        int noteX2 = TimeConverter::xOfTick(note.endTick(), tempoMap, *viewState);

        if (moveX < noteX1 || moveX >= noteX2) {
            continue;
        }

        if (edge) {
            constexpr int kHandlePx = 4;

            if (moveX - noteX1 < kHandlePx) {
                *edge = -1;
            }
            else if (noteX2 - moveX < kHandlePx) {
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
