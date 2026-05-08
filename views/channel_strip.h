#ifndef CHANNEL_STRIP_H
#define CHANNEL_STRIP_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>


class ChannelStrip : public Fl_Group
{
  public:

      ChannelStrip(int x, int y, int w, int h);
      ~ChannelStrip() {}
};

#endif // CHANNEL_STRIP_H



