#include "track.h"
#include "../project/controller.h"

namespace Widget {
    Track::Track(int x, int y, int w, int h, int i, TrackType t, Project::Controller& ctrl) :
      Fl_Group(x, y, w, h), id(i), type(t), projectCtrl(ctrl)
    {
        box(FL_UP_BOX);
        name = new Fl_Output(x + MICRO_SPACE, y + MICRO_SPACE, w - (MICRO_SPACE * 2), MICRO_SPACE * 5);
        add(name);
        // Create the track buttons.
        mute = new Fl_Light_Button(x + MICRO_SPACE, y + MICRO_SPACE * 7, TINY_SPACE * 3, TINY_SPACE * 2, "M");
        add(mute);
        solo = new Fl_Light_Button(x + (MICRO_SPACE * 2) + (TINY_SPACE * 3), y + MICRO_SPACE * 7, TINY_SPACE * 3, TINY_SPACE * 2, "S");
        solo->selection_color(FL_GREEN);
        add(solo);
        record = new Fl_Light_Button(x + (MICRO_SPACE * 3) + (TINY_SPACE * 6), y + MICRO_SPACE * 7, TINY_SPACE * 3, TINY_SPACE * 2, "R");
        record->selection_color(FL_RED);
        add(record);
        // Stop adding children to this widget.
        end();
    }

    Track::~Track()
    {
       // Destructor implementation (can be empty).
       // Prevent errors such as: undefined reference to `vtable for WidgetTrack'
    }

    /*
     * Handles the events happening into the track widget.
     */
    int Track::handle(int event)
    {
        switch (event) {
            case FL_PUSH: {
                if (Fl::event_button() == FL_LEFT_MOUSE) {
                    // Check buttons.
                    if (Fl::event_inside(mute)) {
                        // Check for the button new state.
                        State state = !mute->value() ? State::ON: State::OFF;
                        // Tell the controller the mute button state has changed.
                        projectCtrl.onMuteTrack(getId(), getType(), state);
                    }
                    else if (Fl::event_inside(solo)) {
                        State state = !solo->value() ? State::ON: State::OFF;
                        projectCtrl.onSoloTrack(getId(), getType(), state);
                    }
                    else if (Fl::event_inside(record)) {
                        std::cout << "Track => Record FL_PUSH " << getId() << std::endl;
                    }
                    // The user has clicked on the track itself (ie: the Fl_Group widget) to select it.
                    else {
                        projectCtrl.onTrackSelected(getId());
                        // Event handled. Stop propagation.
                        return 1;
                    }
                }
            }
        }

        // Default - Let Fl_Group handle any not processed events.
        return Fl_Group::handle(event);
    }
}

