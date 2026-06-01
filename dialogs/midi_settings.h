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
      Fl_Choice& getDevice() const { return *device; }
      Fl_Choice& getInput() const { return *input; }
      Fl_Choice& getOutput() const { return *output; }

  private:
      Application& application;
      Fl_Choice* device = nullptr;
      Fl_Choice* input = nullptr;
      Fl_Choice* output = nullptr;

      void buildDevices();
      void buildPorts();
      void onChangeDevice();
      void onChangePort();
      void onButtonsCreated() override;
      void save();
      void cancel();

  protected:
      void buildDialog() override;
      void onOk() override;
};

#endif // MIDI_SETTINGS_H

