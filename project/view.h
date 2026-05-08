#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../main.h"
#include "controller.h"
#include "../views/track_list.h"
#include "../views/channel_strip.h"
#include "../views/timeline.h"

// Forward declarations (for classes outside Project namespace)
class TrackList;

namespace Project {
    // Forward declarations (for classes inside Project namespace)
    class Controller;

    class View : public Fl_Group
    {
      Controller& controller;
      TrackList* trackList = nullptr;
      ChannelStrip* channelStrip = nullptr;
      Timeline* timeline = nullptr;

      public:

          View(int x, int y, int w, int h, Controller& ctrl);
          ~View();
    };
}

#endif // PROJECT_VIEW_H

