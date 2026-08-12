#ifndef MIDI_SETTINGS_H
#define MIDI_SETTINGS_H

#include <FL/Fl_Choice.H>
#include <string>
#include "dialog.h"

// Forward declarations.
class Application;

class MidiSettingsDialog : public Dialog
{
  public:

      MidiSettingsDialog(int x, int y, int width, int height, const char* title, Application& app);
      Fl_Choice& getInput() const { return *input; }
      Fl_Choice& getOutput() const { return *output; }

  private:
      Application& application;
      Fl_Choice* input = nullptr;
      Fl_Choice* output = nullptr;

      void buildPorts();
      void onChangeOutput();
      void onChangeInput();
      void onButtonsCreated() override;

  protected:
      void buildDialog() override;
};

#endif // MIDI_SETTINGS_H
