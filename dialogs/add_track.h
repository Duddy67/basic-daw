#ifndef ADD_TRACK_H
#define ADD_TRACK_H

#include <FL/Fl_Radio_Round_Button.H>
#include <string>
#include "dialog.h"


class AddTrackDialog : public Dialog {
  private:
      Fl_Radio_Round_Button* midi = nullptr;
      Fl_Radio_Round_Button* audio = nullptr;

      struct NewTrackOptions {
          bool midi = true;
      };

      NewTrackOptions options;

  public:
      AddTrackDialog(int x, int y, int width, int height, const char* title);
      NewTrackOptions getOptions() const { return options; }

  protected:
      void buildDialog() override;
      void onOk() override;
};

#endif // ADD_TRACK_H
