#include "channel_strip.h"

ChannelStrip::ChannelStrip(int x, int y, int w, int h) : Fl_Group(x, y, w, h)
{
    box(FL_DOWN_BOX);
    color((Fl_Color) FL_INACTIVE_COLOR);
}


