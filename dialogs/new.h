#ifndef NEW_H
#define NEW_H

#include <FL/Fl_Radio_Round_Button.H>
#include <string>
#include "dialog.h"


class NewDialog : public Dialog {
  private:
      Fl_Radio_Round_Button* cpp = nullptr;
      Fl_Radio_Round_Button* java = nullptr;

      struct NewFileOptions {
          bool cpp = true;
      };

      NewFileOptions options;

  public:
      NewDialog(int x, int y, int width, int height, const char* title);
      NewFileOptions getOptions() const { return options; }

  protected:
      void buildDialog() override;
      void onOk() override;
};

#endif // NEW_H
