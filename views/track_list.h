#ifndef TRACK_LIST_H
#define TRACK_LIST_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../project/observer.h"
#include "../project/controller.h"
#include "../widgets/track.h"

namespace Project {
    class Controller;
    class Observer;
}

namespace Widget {
    class Track;
}

class TrackList : public Fl_Group, public Project::Observer
{
    Project::Controller& projectCtrl;
    std::vector<Widget::Track*> tracks;

  public:

      TrackList(int x, int y, int w, int h, Project::Controller& ctrl);
      ~TrackList() {}

      void onCtrlEvent(CtrlEvent event, int index);
      void addTrack(int id, TrackType type);
};

#endif // TRACK_LIST_H



