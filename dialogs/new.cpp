#include "new.h"

NewDialog::NewDialog(int x, int y, int width, int height, const char* title) 
  : Dialog(x, y, width, height, title)
{
    init();
}

void NewDialog::buildDialog()
{
    // Create the radio button options.
    cpp = new Fl_Radio_Round_Button(MODAL_WND_POS, MODAL_WND_POS, BUTTON_WIDTH, BUTTON_HEIGHT, "C++");
    java = new Fl_Radio_Round_Button(MODAL_WND_POS, MODAL_WND_POS + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, "Java");
    // Set cpp option as default.
    cpp->set();

    // Add the Ok/Cancel buttons.
    addDefaultButtons();
}

void NewDialog::onOk()
{
    // Set the option values chosen by the user (ie: cpp/java).
    options.cpp = cpp->value() ? true : false;

    Dialog::onOk();
}
