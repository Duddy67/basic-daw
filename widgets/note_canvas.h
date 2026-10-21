#ifndef NOTE_CANVAS_H
#define NOTE_CANVAS_H

#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <string.h>
#include "../project/controller.h"
#include "../midi/clip.h"
#include "../constants.h"


namespace Project {
    class Controller;
}

class NoteCanvas : public Fl_Widget
{
    Project::Controller& projectCtrl;
    // Pointer to share state with other GUI elements.
    ViewState* viewState;
    DragMode mode = DragMode::NONE;
    // Widget-relative mouse at FL_PUSH.
    int dragStartX = 0;
    int dragStartY = 0;
    // Snapshot at FL_PUSH.
    Note dragOriginal;
    // Index into clip->getNotes() during a drag.
    int dragNoteIdx = -1;

    int64_t snap(int64_t tick);
    Midi::Clip* currentClip();
    int hitTest(int mx, int my, int* edge) const;

  protected:
    void draw() override;
    int handle(int event) override;

  public:

    NoteCanvas(int x, int y, int w, int h, Project::Controller& ctrl);
    ~NoteCanvas() {}

    int64_t minNoteLengthTicks() const;
};

#endif // NOTE_CANVAS_H



