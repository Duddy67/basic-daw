#ifndef TRACK_LIST_H
#define TRACK_LIST_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../project/observer.h"
#include "../project/controller.h"

namespace Project {
    class Controller;
    class Observer;
}

class TrackList : public Fl_Group, public Project::Observer
{
    Project::Controller& projectCtrl;

  public:

      TrackList(int x, int y, int w, int h, Project::Controller& ctrl);
      ~TrackList() {}

      void onEvent(ProjectEvent event, int index);
};

#endif // TRACK_LIST_H



