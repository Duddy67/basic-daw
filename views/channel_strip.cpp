#include "channel_strip.h"

ChannelStrip::ChannelStrip(int x, int y, int w, int h, Project::Controller& ctrl) : Fl_Group(x, y, w, h), projectCtrl(ctrl)
{
    box(FL_DOWN_BOX);
    color((Fl_Color) FL_INACTIVE_COLOR);
    projectCtrl.addObserver(this);
    end();
}


void ChannelStrip::onCtrlEvent(CtrlEvent event, int index)
{
    switch (event) {
        case CtrlEvent::ADD_MIDI_TRACK:
          addStrip(index, TrackType::MIDI);
          std::cout << "ChannelStrip => ADD_MIDI_TRACK" << std::endl;
          break;

        case CtrlEvent::ADD_AUDIO_TRACK:
          addStrip(index, TrackType::AUDIO);
          std::cout << "ChannelStrip => ADD_AUDIO_TRACK" << std::endl;
          break;

        case CtrlEvent::REMOVE_TRACK:
          std::cout << "ChannelStrip => REMOVE_TRACK" << std::endl;
          break;

        case CtrlEvent::TRACK_SELECTED:
          std::cout << "ChannelStrip => TRACK_SELECTED" << std::endl;
          showCurrentStrip(index);
          redraw();
          break;

        // For cases that are not managed.
        default:

          return;
    }
}

void ChannelStrip::addStrip(int id, TrackType type)
{
    int x = this->x() + BORDER_INTERSTICE;
    int y = this->y() + BORDER_INTERSTICE;

    Strip* strip = new Strip(x, y, w() - (BORDER_INTERSTICE * 2), h() - (BORDER_INTERSTICE * 2), id, type, projectCtrl);
    // Add the new strip as ChannelStrip's child.
    add(strip);
    // Add the new strip to the list.
    strips.push_back(strip);
    showCurrentStrip(id);
    redraw();
    //std::cout << "ChannelStrip => ADD_TRACK " << " " << strip->x() << " " << strip->y() << std::endl;
}

void ChannelStrip::showCurrentStrip(int id)
{
    for (auto* strip : strips) {
        // Hide all the strip in the list.
        strip->hide();

        // Except for the given strip.
        if (strip->getId() == id) {
            strip->show();
        }
    }
}
