#include "midi_settings.h"
#include "../main.h"

MidiSettingsDialog::MidiSettingsDialog(int x, int y, int width, int height, const char* title, Application& app) 
  : Dialog(x, y, width, height, title), application(app)
{
    init();
}


/*
 * Create the setting's backend and port drop down lists.
 */
void MidiSettingsDialog::buildDialog()
{
    // Drop down list height.
    int height = (TINY_SPACE * 2) + MICRO_SPACE;

    output = new Fl_Choice(SMALL_SPACE * 3, TINY_SPACE * 3, MEDIUM_SPACE * 3, height, "Output => ");
    input = new Fl_Choice(SMALL_SPACE * 3, (TINY_SPACE * 6), MEDIUM_SPACE * 3, height, "Input => ");

    // Align labels.
    output->align(FL_ALIGN_LEFT);
    input->align(FL_ALIGN_LEFT);

    output->callback([](Fl_Widget*, void* userdata) {
        static_cast<MidiSettingsDialog*>(userdata)->onChangeOutput();
    }, this);

    input->callback([](Fl_Widget*, void* userdata) {
        static_cast<MidiSettingsDialog*>(userdata)->onChangeInput();
    }, this);

    if (!application.getCoreEngine().isClientActivated()) {
        std::cerr << "Midi system not initialized." << std::endl;
    }
    else {
        buildPorts();
    }

    // Add the Ok/Cancel default buttons.
    addDefaultButtons();
}

void MidiSettingsDialog::onButtonsCreated()
{
    // Change the Cancel button's label.
    cancelButton->label("Close");
    // No need Ok button.
    okButton->hide();
}

/*
 * Builds an option list for each midi port of the application.
 */
void MidiSettingsDialog::buildPorts()
{
    auto config = loadConfig();

    auto outputPorts = application.getCoreEngine().getOutputPorts(DataType::MIDI);
    int value = -1;
    std::string escapedName = "";

    for (size_t i = 0; i < outputPorts.size(); ++i) {
        // Create an option for the port.
        escapedName = application.escapeMenuText(outputPorts[i]);
        output->add(escapedName.c_str());

        if (config.midi.outputPort.compare(outputPorts[i].c_str()) == 0) {
            value = i;
        }
    }

    if (value > -1) {
        output->value(value);
    }

    auto inputPorts = application.getCoreEngine().getInputPorts(DataType::MIDI);
    value = -1;

    for (size_t i = 0; i < inputPorts.size(); ++i) {
        // Create an option for the port.
        escapedName = application.escapeMenuText(inputPorts[i]);
        input->add(escapedName.c_str());

        if (config.midi.inputPort.compare(inputPorts[i].c_str()) == 0) {
            value = i;
        }
    }

    if (value > -1) {
        input->value(value);
    }
}

/*
 * The output option has been changed.
 */
void MidiSettingsDialog::onChangeOutput()
{
    auto config = loadConfig();
    // Get the application's output port name.
    const char* portName = application.getCoreEngine().getMidiPortName(ConnectionType::OUTPUT);

    // Get the old selection from the config file.
    const char* oldSelection = config.midi.outputPort.c_str();
    const char* newSelection = output->text();
    // Cancel the previous connection first.
    application.getCoreEngine().safeDisconnect(portName, oldSelection);
    // Set the new connection.
    application.getCoreEngine().safeConnect(portName, newSelection);

    // Store the new connection in the config file.
    config.midi.outputPort = newSelection;

    saveConfig();
}

/*
 * The input option has been changed.
 */
void MidiSettingsDialog::onChangeInput()
{
    auto config = loadConfig();
    // Get the application's input port name.
    const char* portName = application.getCoreEngine().getMidiPortName(ConnectionType::INPUT);

    // Get the old selection from the config file.
    const char* oldSelection = config.midi.inputPort.c_str();
    const char* newSelection = input->text();
    // Cancel the previous connection first.
    application.getCoreEngine().safeDisconnect(portName, oldSelection);
    // Set the new connection.
    application.getCoreEngine().safeConnect(portName, newSelection);

    // Store the new connection in the config file.
    config.midi.inputPort = newSelection;

    saveConfig();
}

