#include "audio_settings.h"
#include "../main.h"

AudioSettingsDialog::AudioSettingsDialog(int x, int y, int width, int height, const char* title, Application& app) 
  : Dialog(x, y, width, height, title), application(app)
{
    init();
}

/*
 * Create the setting's backend and port drop down lists.
 */
void AudioSettingsDialog::buildDialog()
{
    // Drop down list height.
    int height = (TINY_SPACE * 2) + MICRO_SPACE;

    outputLeft = new Fl_Choice(SMALL_SPACE * 3, TINY_SPACE * 3, MEDIUM_SPACE * 3, height, "Out left => ");
    outputRight = new Fl_Choice(SMALL_SPACE * 3, (TINY_SPACE * 6), MEDIUM_SPACE * 3, height, "Out right => ");
    inputLeft = new Fl_Choice(SMALL_SPACE * 3, (TINY_SPACE * 2) * 5 + TINY_SPACE, MEDIUM_SPACE * 3, height, "In left => ");
    inputRight = new Fl_Choice(SMALL_SPACE * 3, (TINY_SPACE * 2) * 6 + (TINY_SPACE * 2), MEDIUM_SPACE * 3, height, "Out left => ");

    // Align labels.
    outputLeft->align(FL_ALIGN_LEFT);
    outputRight->align(FL_ALIGN_LEFT);
    inputLeft->align(FL_ALIGN_LEFT);
    inputRight->align(FL_ALIGN_LEFT);

    outputLeft->callback([](Fl_Widget*, void* userdata) {
        static_cast<AudioSettingsDialog*>(userdata)->onChangeOutput(Direction::LEFT);
    }, this);

    outputRight->callback([](Fl_Widget*, void* userdata) {
        static_cast<AudioSettingsDialog*>(userdata)->onChangeOutput(Direction::RIGHT);
    }, this);

    inputLeft->callback([](Fl_Widget*, void* userdata) {
        static_cast<AudioSettingsDialog*>(userdata)->onChangeInput(Direction::LEFT);
    }, this);

    inputRight->callback([](Fl_Widget*, void* userdata) {
        static_cast<AudioSettingsDialog*>(userdata)->onChangeInput(Direction::RIGHT);
    }, this);

    if (!application.getCoreEngine().isClientActivated()) {
        std::cerr << "Audio system not initialized." << std::endl;
    }
    else {
        buildPorts();
    }

    // Add the Ok/Cancel default buttons.
    addDefaultButtons();
}

void AudioSettingsDialog::onButtonsCreated()
{
    // Change the Cancel button's label.
    cancelButton->label("Close");
    // No need Ok button.
    okButton->hide();
}

/*
 * Builds an option list for each port of the application.
 */
void AudioSettingsDialog::buildPorts()
{
    auto config = loadConfig();

    auto outputPorts = application.getCoreEngine().getOutputPorts(DataType::AUDIO);
    int valueOutL = -1, valueOutR = -1;
    std::string escapedName = "";

    for (size_t i = 0; i < outputPorts.size(); ++i) {
        // Create an option for the device.
        escapedName = application.escapeMenuText(outputPorts[i]);
        outputLeft->add(escapedName.c_str());
        outputRight->add(escapedName.c_str());

        if (config.audio.outputLeft.compare(outputPorts[i].c_str()) == 0) {
            valueOutL = i;
        }

        if (config.audio.outputRight.compare(outputPorts[i].c_str()) == 0) {
            valueOutR = i;
        }
    }

    if (valueOutL > -1) {
        outputLeft->value(valueOutL);
    }

    if (valueOutR > -1) {
        outputRight->value(valueOutR);
    }

    auto inputPorts = application.getCoreEngine().getInputPorts(DataType::AUDIO);
    int valueInL = -1, valueInR = -1;

    for (size_t i = 0; i < inputPorts.size(); ++i) {
        // Create an option for the device.
        escapedName = application.escapeMenuText(inputPorts[i]);
        inputLeft->add(escapedName.c_str());
        inputRight->add(escapedName.c_str());

        if (config.audio.inputLeft.compare(inputPorts[i].c_str()) == 0) {
            valueInL = i;
        }

        if (config.audio.inputRight.compare(inputPorts[i].c_str()) == 0) {
            valueInR = i;
        }
    }

    if (valueInL > -1) {
        inputLeft->value(valueInL);
    }

    if (valueInR > -1) {
        inputRight->value(valueInR);
    }
}

/*
 * The left or right output option has been changed.
 */
void AudioSettingsDialog::onChangeOutput(Direction direction)
{
    auto config = loadConfig();
    // Get the application's output port name.
    const char* portName = (direction == Direction::LEFT)
                           ? application.getCoreEngine().getAudioPortName(ConnectionType::OUTPUT, Direction::LEFT)
                           : application.getCoreEngine().getAudioPortName(ConnectionType::OUTPUT, Direction::RIGHT);
    // Get the old selection from the config file.
    const char* oldSelection = direction == Direction::LEFT ? config.audio.outputLeft.c_str() : config.audio.outputRight.c_str();
    const char* newSelection = direction == Direction::LEFT ? outputLeft->text() : outputRight->text();
    // Cancel the previous connection first.
    application.getCoreEngine().safeDisconnect(portName, oldSelection);
    // Set the new connection.
    application.getCoreEngine().safeConnect(portName, newSelection);

    // Store the new connection in the config file.
    if (direction == Direction::LEFT) {
        config.audio.outputLeft = newSelection;
    }
    else {
        config.audio.outputRight = newSelection;
    }

    saveConfig();
}

/*
 * The left or right input option has been changed.
 */
void AudioSettingsDialog::onChangeInput(Direction direction)
{
    auto config = loadConfig();
    // Get the application's input port name.
    const char* portName = (direction == Direction::LEFT)
                           ? application.getCoreEngine().getAudioPortName(ConnectionType::INPUT, Direction::LEFT)
                           : application.getCoreEngine().getAudioPortName(ConnectionType::INPUT, Direction::RIGHT);
    // Get the old selection from the config file.
    const char* oldSelection = direction == Direction::LEFT ? config.audio.inputLeft.c_str() : config.audio.inputRight.c_str();
    const char* newSelection = direction == Direction::LEFT ? inputLeft->text() : inputRight->text();
    // Cancel the previous connection first.
    application.getCoreEngine().safeDisconnect(oldSelection, portName);
    // Set the new connection.
    application.getCoreEngine().safeConnect(newSelection, portName);

    // Store the new connection in the config file.
    if (direction == Direction::LEFT) {
        config.audio.inputLeft = newSelection;
    }
    else {
        config.audio.inputRight = newSelection;
    }

    saveConfig();
}

