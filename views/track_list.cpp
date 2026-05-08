#include "track_list.h"

TrackList::TrackList(int x, int y, int w, int h, Project::Controller& ctrl) : Fl_Group(x, y, w, h), projectCtrl(ctrl)
{
    box(FL_DOWN_BOX);
    color((Fl_Color) FL_INACTIVE_COLOR);
    projectCtrl.addObserver(this);
}


void TrackList::onEvent(ProjectEvent event, int index)
{
    switch (event) {
        case ProjectEvent::ADD_TRACK:
          std::cout << "TrackList => ADD_TRACK" << std::endl;
          break;

        case ProjectEvent::REMOVE_TRACK:
          std::cout << "TrackList => REMOVE_TRACK" << std::endl;
          break;
    }

}
