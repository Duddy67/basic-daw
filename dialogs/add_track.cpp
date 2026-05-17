#include "add_track.h"

AddTrackDialog::AddTrackDialog(int x, int y, int width, int height, const char* title) 
  : Dialog(x, y, width, height, title)
{
    init();
}

void AddTrackDialog::buildDialog()
{
    // Create the radio button options.
    midi = new Fl_Radio_Round_Button(MODAL_WND_POS, MODAL_WND_POS, BUTTON_WIDTH, BUTTON_HEIGHT, "MIDI");
    audio = new Fl_Radio_Round_Button(MODAL_WND_POS, MODAL_WND_POS + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, "Audio");
    // Set midi option as default.
    midi->set();

    // Add the Ok/Cancel buttons.
    addDefaultButtons();
}

void AddTrackDialog::onOk()
{
    // Set the option values chosen by the user (ie: midi/audio).
    options.midi = midi->value() ? true : false;

    Dialog::onOk();
}
