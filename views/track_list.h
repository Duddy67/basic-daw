#ifndef TRACK_LIST_H
#define TRACK_LIST_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../project/observer.h"
#include "../project/controller.h"
#include "../widgets/track.h"

namespace project {
    class Controller;
    class Observer;
}

namespace Widget {
    class Track;
}

class TrackList : public Fl_Group, public project::Observer
{
    project::Controller& projectCtrl;
    std::vector<Widget::Track*> tracks;
    void setSelectedTrack(int id);

  public:

      TrackList(int x, int y, int w, int h, project::Controller& ctrl);
      ~TrackList() {}

      void onCtrlEvent(CtrlEvent event, int index);
      void addTrack(int id, DataType type);
      Widget::Track& getSelectedTrack();
};

#endif // TRACK_LIST_H



