#ifndef TIMELINE_H
#define TIMELINE_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Box.H>


class Timeline : public Fl_Group
{
    Fl_Scrollbar* vScrollbar = nullptr;
    Fl_Scrollbar* hScrollbar = nullptr;

  public:

      Timeline(int x, int y, int w, int h);
      ~Timeline() {}
};

#endif // TIMELINE_H



