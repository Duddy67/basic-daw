#include "track_list.h"

TrackList::TrackList(int x, int y, int w, int h, Project::Controller& ctrl) : Fl_Group(x, y, w, h), projectCtrl(ctrl)
{
    box(FL_DOWN_BOX);
    color((Fl_Color) FL_INACTIVE_COLOR);
    projectCtrl.addObserver(this);
}


void TrackList::onCtrlEvent(CtrlEvent event, int index)
{
    switch (event) {
        case CtrlEvent::ADD_TRACK:
          addTrack();
          break;

        case CtrlEvent::REMOVE_TRACK:
          std::cout << "TrackList => REMOVE_TRACK" << std::endl;
          break;
    }

}

void TrackList::addTrack()
{
    int x, y;

    if (tracks.empty()) {
        // Position the track at the top of the list.
        x = this->x() + BORDER_INTERSTICE;
        y = this->y() + BORDER_INTERSTICE;
    }
    else {
        auto lastTrack = tracks.back();
        x = lastTrack->x();
        y = lastTrack->y() + MEDIUM_SPACE + BORDER_INTERSTICE;
    }

    Widget::Track* track = new Widget::Track(x, y, w() - (BORDER_INTERSTICE * 2), MEDIUM_SPACE);
    // Add the new track as TrackList's child.
    add(track);
    // Add the new track to the list.
    tracks.push_back(track);
    redraw();
    std::cout << "TrackList => ADD_TRACK " << " " << track->x() << " " << track->y() << std::endl;
}

