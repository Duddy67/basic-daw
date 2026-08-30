#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "../main.h"
#include "controller.h"
#include "../views/track_list.h"
#include "../views/channel_strip.h"
#include "../views/timeline.h"
#include "../views/ruler.h"

// Forward declarations (for classes outside the Project namespace)
class TrackList;
class ChannelStrip;
class Timeline;

namespace Project {
    // Forward declarations (for classes inside the Project namespace)
    class Controller;

    class View : public Fl_Group
    {
            Controller& controller;
            TrackList* trackList = nullptr;
            ChannelStrip* channelStrip = nullptr;
            Timeline* timeline = nullptr;
            Ruler* ruler = nullptr;
            bool isLiveUpdating = false;

            static void liveUpdate_cb(void* userData);

        public:

            View(int x, int y, int w, int h, Controller& ctrl);
            ~View();

            void startLiveUpdate();
            void stopLiveUpdate();
    };
}

#endif // PROJECT_VIEW_H

