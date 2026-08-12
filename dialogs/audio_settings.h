#ifndef AUDIO_SETTINGS_H
#define AUDIO_SETTINGS_H

#include <FL/Fl_Choice.H>
#include <string>
#include "dialog.h"

// Forward declarations.
class Application;

class AudioSettingsDialog : public Dialog
{
  public:

      AudioSettingsDialog(int x, int y, int width, int height, const char* title, Application& app);
      Fl_Choice& getInput(Direction direction) const { return direction == Direction::LEFT ? *inputLeft : *inputRight; }
      Fl_Choice& getOutput(Direction direction) const { return direction == Direction::LEFT ? *outputLeft : *outputRight; }

  private:
      Application& application;
      Fl_Choice* inputLeft = nullptr;
      Fl_Choice* outputLeft = nullptr;
      Fl_Choice* inputRight = nullptr;
      Fl_Choice* outputRight = nullptr;

      void buildPorts();
      void onChangeOutput(Direction direction);
      void onChangeInput(Direction direction);
      void onButtonsCreated() override;

  protected:
      void buildDialog() override;
};

#endif // AUDIO_SETTINGS_H

