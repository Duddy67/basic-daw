#include "keyboard.h"


Keyboard::Keyboard(int x, int y, int w, int h, Project::Controller& ctrl)
    : Fl_Widget(x, y, w, h), projectCtrl(ctrl)
{
    box(FL_FLAT_BOX);
    viewState = &projectCtrl.getViewState();

}

void Keyboard::draw()
{
    // Prevents drawing outside the widget boundaries.
    fl_push_clip(x(), y(), w(), h());

    // Clear the background.
    fl_color((Fl_Color)  FL_LIGHT1);
    fl_rectf(x(), y(), w(), h());

    int top = PitchConverter::pitchAtY(y(), y(), h(), *viewState);
    int bottom = PitchConverter::pitchAtY(y() + h() - 1, y(), h(), *viewState);

    for (int pitch = bottom; pitch <= top && pitch <= 127; ++pitch) {

        if (pitch < 0) {
            continue;
        }

        int keyY = PitchConverter::yOfPitch(pitch, y(), h(), *viewState);
        int keyH = viewState->keyHeight;

        // Set the key color.

        Fl_Color color = PitchConverter::isBlackKey(pitch) ? FL_BLACK : FL_WHITE;

        // The key is clicked.
        if (pitch == playingPitch) {
            color = FL_RED;
        }
        // The key is hovered.
        else if (pitch == hoveredPitch) {
            color = FL_DARK1;
        }

        fl_color(color);
        fl_rectf(x(), keyY, w(), keyH);
        fl_color(FL_GRAY0);
        fl_rect(x(), keyY, w(), keyH);

        // Draw "C" labels on C rows.
        if (!PitchConverter::isBlackKey(pitch) && (pitch % 12) == 0) {
            fl_color(FL_BLACK);
            fl_draw(("C" + std::to_string(pitch / 12 - 1)).c_str(), x() + 2, keyY + keyH - 2);
        }
    }

    fl_pop_clip();
}

int Keyboard::handle(int event)
{
    int pitch = PitchConverter::pitchAtY(Fl::event_y(), y(), h(), *viewState);

    switch (event) {
        case FL_ENTER:
            return 1;

        case FL_LEAVE:
            hoveredPitch = -1;
            redraw();
            return 1;

        case FL_MOVE:
            if (pitch != hoveredPitch) {
                hoveredPitch = pitch;
                redraw();
            }

            return 1;

        case FL_PUSH:
            {
                // Only handle left click.
                if (Fl::event_button() != FL_LEFT_MOUSE) {
                    // Right click or other buttons not handled. Let parent widgets see it too.
                    return 0;
                }

                int velocity = velocityAtX(Fl::event_x());

                // Zero by MIDI convention means Note Off.
                if (velocity == 0) {
                    // Skip it.
                    return 1;
                }

                playingPitch = pitch;
                projectCtrl.onPreviewNoteOn(pitch, velocity);
                redraw();

                return 1;
            }

        case FL_DRAG:
            {
                if (pitch != playingPitch) {
                    int velocity = velocityAtX(Fl::event_x());

                    // Zero by MIDI convention means Note Off.
                    if (velocity == 0) {
                        // Skip it.
                        return 1;
                    }

                    projectCtrl.onPreviewNoteOff(playingPitch);
                    playingPitch = pitch;
                    projectCtrl.onPreviewNoteOn(pitch, velocity);
                    redraw();
                }

                return 1;
            }

        case FL_RELEASE:
            projectCtrl.onPreviewNoteOff(playingPitch);
            playingPitch = -1;
            redraw();

            return 1;
    }

    return 0;
}

/*
 * Converts the clicked position on a key (left <-> right) into 
 * a velocity value (1 <-> 127).
 */
int Keyboard::velocityAtX(int eventX) const
{
    // Check for clicks at right edge position.
    if (w() <= 1) {
        // Return max velocity value.
        return 127;
    }

    int rel = std::clamp(eventX - x(), 0, w() - 1);

    return (int)std::round(rel * 127.0 / w() - 1);
}

