#include "midi_settings.h"
#include "../main.h"

MidiSettingsDialog::MidiSettingsDialog(int x, int y, int width, int height, const char* title, Application& app) 
  : Dialog(x, y, width, height, title), application(app)
{
    init();
}


/*
 * Create the setting's device and port drop down lists.
 */
void MidiSettingsDialog::buildDialog()
{
    // Drop down list height.
    int height = (TINY_SPACE * 2) + MICRO_SPACE;

    device = new Fl_Choice(SMALL_SPACE, TINY_SPACE * 3, XLARGE_SPACE, height, "Device");
    output = new Fl_Choice(SMALL_SPACE, (TINY_SPACE * 2) * 4, XLARGE_SPACE, height, "Output Port");
    input = new Fl_Choice(SMALL_SPACE, (TINY_SPACE * 2) * 6 + TINY_SPACE, XLARGE_SPACE, height, "Input Port");
    // Align labels.
    device->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
    output->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
    input->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    device->callback([](Fl_Widget*, void* userdata) {
        static_cast<MidiSettingsDialog*>(userdata)->onChangeDevice();
    }, this);

    input->callback([](Fl_Widget*, void* userdata) {
        static_cast<MidiSettingsDialog*>(userdata)->onChangePort();
    }, this);

    output->callback([](Fl_Widget*, void* userdata) {
        static_cast<MidiSettingsDialog*>(userdata)->onChangePort();
    }, this);

    buildDevices();
    buildPorts();

    // Add the Ok/Cancel buttons.
    addDefaultButtons();
}

void MidiSettingsDialog::onButtonsCreated() 
{
    // No need cancel button.
    cancelButton->hide();
    // Reposition the ok button at the bottom middle.
    okButton->position((window->h() / 2) + (BUTTON_WIDTH / 2), okButton->y());
}

void MidiSettingsDialog::onOk()
{
    // Just close the dialog box.
    Dialog::onOk();
}

void MidiSettingsDialog::buildDevices()
{
    // Get the required variables.
    auto devices = application.getMidiEngine().getDevices();
    AppConfig& config = loadConfig();

    for (size_t i = 0; i < devices.size(); ++i) {
        // Create an entry for each available device.
        device->add(devices[i].c_str());

        // Set to the default device. 
        if (config.midi.device.compare(devices[i].c_str()) == 0) {
            device->value(i);
        }
    }
}

void MidiSettingsDialog::buildPorts()
{
    AppConfig& config = loadConfig();
    auto inputs = application.getMidiEngine().getInputPorts();

    for (size_t i = 0; i < inputs.size(); ++i) {
        // Create an entry for each available input port.
        input->add(inputs[i].c_str());

        // Set to the default input. 
        if (config.midi.inputPort.compare(inputs[i].c_str()) == 0) {
            input->value(i);
        }
    }

    auto outputs = application.getMidiEngine().getOutputPorts();

    for (size_t i = 0; i < outputs.size(); ++i) {
        // Create an entry for each available output port.
        output->add(outputs[i].c_str());

        // Set to the default input. 
        if (config.midi.outputPort.compare(outputs[i].c_str()) == 0) {
            output->value(i);
        }
    }
}

void MidiSettingsDialog::onChangeDevice() 
{
    // Reinitialize midi device.
    try {
        application.getMidiEngine().initDevice(device->text());
    }
    catch (RtMidiError& e) {
        std::cerr << "RtMidi error: " << std::string(e.what()) << std::endl;
        return;
    }

    // Reinitialize midi ports.

    try {
        application.getMidiEngine().initInputPort();
    }
    catch (RtMidiError& e) {
        std::cerr << "RtMidi error: " << std::string(e.what()) << std::endl;
        return;
    }

    try {
        application.getMidiEngine().initOutputPort();
    }
    catch (RtMidiError& e) {
        std::cerr << "RtMidi error: " << std::string(e.what()) << std::endl;
        return;
    }

    // Delete the previous port options.
    input->clear();
    output->clear();

    // Rebuild the port options.
    buildPorts();
    window->redraw();
}

void MidiSettingsDialog::onChangePort() 
{
    // Reinitialize midi ports.

    try {
        application.getMidiEngine().initInputPort(input->text());
    }
    catch (RtMidiError& e) {
        std::cerr << "RtMidi error: " << std::string(e.what()) << std::endl;
        return;
    }

    try {
        application.getMidiEngine().initOutputPort(output->text());
    }
    catch (RtMidiError& e) {
        std::cerr << "RtMidi error: " << std::string(e.what()) << std::endl;
        return;
    }
}

