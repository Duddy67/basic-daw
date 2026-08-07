#include "strip.h"
#include <iostream>

Strip::Strip(int x, int y, int w, int h, int i, DataType t, Project::Controller& ctrl) :
  Fl_Group(x, y, w, h), id(i), type(t), projectCtrl(ctrl)
{
    box(FL_UP_BOX);

    // Stop adding children (Important!).
    end();

    if (type == DataType::MIDI) {
        buildMidi();
    }
    else {
        buildAudio();
    }

    show();
}

Strip::~Strip()
{
   // Destructor implementation (can be empty).
   // Prevent errors such as: undefined reference to `vtable for WidgetTrack'
}

void Strip::buildMidi()
{
        std::cout << "buildMidi " << std::endl;
    int height = (TINY_SPACE * 2) + MICRO_SPACE;
    channel = new Fl_Choice(x() + MICRO_SPACE, y() + (SMALL_SPACE / 2), LARGE_SPACE / 2, height, "Channel");

    for (unsigned int i = 0; i < MAX_MIDI_CHANNELS; i++) {
        int channelNb = i + 1;
        std::stringstream t;
        t << channelNb;
        char const* text = t.str().c_str();
        channel->add(text);
    }

    // Set midi channel to 1 by default (zero actually).
    channel->value(0);

    // Align labels.
    channel->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
    add(channel);

    omni = new Fl_Check_Button(x() + MICRO_SPACE + LARGE_SPACE, y() + (SMALL_SPACE / 2), TINY_SPACE, TINY_SPACE, "Omni");
    omni->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
    add(omni);
}

void Strip::buildAudio()
{

}

