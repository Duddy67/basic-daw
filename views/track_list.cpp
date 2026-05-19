#include "track_list.h"

TrackList::TrackList(int x, int y, int w, int h, Project::Controller& ctrl) : Fl_Group(x, y, w, h), projectCtrl(ctrl)
{
    box(FL_DOWN_BOX);
    color((Fl_Color) FL_INACTIVE_COLOR);
    projectCtrl.addObserver(this);
    end();
}


void TrackList::onCtrlEvent(CtrlEvent event, int index)
{
    switch (event) {
        case CtrlEvent::ADD_MIDI_TRACK:
          addTrack(index, TrackType::MIDI);
          break;

        case CtrlEvent::ADD_AUDIO_TRACK:
          addTrack(index, TrackType::AUDIO);
          break;

        case CtrlEvent::REMOVE_TRACK:
          std::cout << "TrackList => REMOVE_TRACK" << std::endl;
          break;

        case CtrlEvent::SOLOED_TRACK:
          std::cout << "TrackList => SOLOED_TRACK" << std::endl;
          break;

        case CtrlEvent::UNSOLOED_TRACK:
          std::cout << "TrackList => UNSOLOED_TRACK" << std::endl;
          break;

        case CtrlEvent::TRACK_SELECTED:
          std::cout << "TrackList => TRACK_SELECTED" << std::endl;
          setSelectedTrack(index);
          redraw();
          break;

        // Cases that are not managed.
        default:

          return;
    }
}

void TrackList::addTrack(int id, TrackType type)
{
    int x, y;

    if (tracks.empty()) {
        // Position the track at the top of the list.
        x = this->x() + BORDER_INTERSTICE;
        y = this->y() + BORDER_INTERSTICE;
    }
    else {
        // Position the new track below the last track of the list.
        auto lastTrack = tracks.back();
        x = lastTrack->x();
        y = lastTrack->y() + MEDIUM_SPACE + BORDER_INTERSTICE;
    }

    Widget::Track* track = new Widget::Track(x, y, w() - (BORDER_INTERSTICE * 2), MEDIUM_SPACE, id, type, projectCtrl);
    // Add the new track as TrackList's child.
    add(track);
    // Add the new track to the list.
    tracks.push_back(track);
    // Set the new track as selected.
    setSelectedTrack(id);

    redraw();
}

void TrackList::setSelectedTrack(int id)
{
    for (auto* track : tracks) {
        // Unselect all the track in the list.
        track->unselect();
        track->color(FL_GRAY);

        // Except for the given track.
        if (track->getId() == id) {
            track->select();
            track->color(FL_LIGHT2);
        }
    }
}
