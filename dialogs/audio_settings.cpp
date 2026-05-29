#include "audio_settings.h"
#include "../main.h"

AudioSettingsDialog::AudioSettingsDialog(int x, int y, int width, int height, const char* title, Application& app) 
  : Dialog(x, y, width, height, title), application(app)
{
    init();
}

/*
 * Create the setting's backend and device drop down lists.
 */
void AudioSettingsDialog::buildDialog()
{
    // Drop down list height.
    int height = (TINY_SPACE * 2) + MICRO_SPACE;

    backend = new Fl_Choice(SMALL_SPACE, TINY_SPACE * 3, XLARGE_SPACE, height, "Backend");
    output = new Fl_Choice(SMALL_SPACE, (TINY_SPACE * 2) * 4, XLARGE_SPACE, height, "Output");
    input = new Fl_Choice(SMALL_SPACE, (TINY_SPACE * 2) * 6 + TINY_SPACE, XLARGE_SPACE, height, "Input");
    // Align labels.
    backend->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
    output->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
    input->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    backend->callback([](Fl_Widget*, void* userdata) {
        static_cast<AudioSettingsDialog*>(userdata)->onChangeBackend();
    }, this);

    output->callback([](Fl_Widget*, void* userdata) {
        static_cast<AudioSettingsDialog*>(userdata)->onChangeOutput();
    }, this);

    if (!application.getAudioEngine().isContextInitialized()) {
        std::cerr << "Failed to initialize audio system." << std::endl;
    }
    else {
        buildBackends();
        buildDevices();
    }

    // Add the Ok/Cancel buttons.
    addDefaultButtons();
}

void AudioSettingsDialog::onButtonsCreated() 
{
    // Change the OK button's label.
    okButton->label("Save");   
}

void AudioSettingsDialog::onOk()
{
    save();
    Dialog::onOk();
}

void AudioSettingsDialog::onCancel()
{
    cancel();
    Dialog::onCancel();
}

void AudioSettingsDialog::buildBackends()
{
    // Get the required variables.
    auto backends = application.getAudioEngine().getBackends();
    AppConfig& config = loadConfig();

    for (size_t i = 0; i < backends.size(); ++i) {
        backend->add(backends[i].name.c_str());

        if (config.audio.backend.compare(backends[i].name.c_str()) == 0) {
            backend->value(i);
        }
    }
}

void AudioSettingsDialog::buildDevices()
{
    AppConfig& config = loadConfig();

    auto outputDevices = application.getAudioEngine().getOutputDevices();
    int value = 100, defaultDevice = 0;
    std::string escapedName = ""; 

    for (size_t i = 0; i < outputDevices.size(); ++i) {
        // Create an option for the device.
        escapedName = application.escapeMenuText(outputDevices[i].name);
        output->add(escapedName.c_str());

        if (outputDevices[i].isDefault) {
            defaultDevice = i;
        }

        if (config.audio.outputDevice.compare(outputDevices[i].name.c_str()) == 0) {
            value = i;
        }
    }

    value = (value != 100) ? value : defaultDevice;
    output->value(value);

    auto inputDevices = application.getAudioEngine().getInputDevices();
    value = 100, defaultDevice = 0;

    for (size_t i = 0; i < inputDevices.size(); ++i) {
        // Create an option for the device.
        escapedName = application.escapeMenuText(inputDevices[i].name);
        input->add(escapedName.c_str());

        if (inputDevices[i].isDefault) {
            defaultDevice = i;
        }

        if (config.audio.inputDevice.compare(inputDevices[i].name.c_str()) == 0) {
            value = i;
        }
    }

    value = (value != 100) ? value : defaultDevice;
    input->value(value);
}

/*
 * The backend option has been changed.
 */
void AudioSettingsDialog::onChangeBackend()
{
    //Application* pApplication = (Application*) data;

    try {
        // Reset backend and devices.
        application.getAudioEngine().setBackend(backend->text());
        application.getAudioEngine().setOutputDevice(output->text());
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Backend choice error: " << std::string(e.what()) << std::endl;
        return;
    }

    // Delete the previous device options.
    output->clear();
    input->clear();
    // Rebuild the device options.
    buildDevices();
}

/*
 * The output option has been changed.
 */
void AudioSettingsDialog::onChangeOutput()
{
    try {
        // Reset the output device.
        application.getAudioEngine().setOutputDevice(output->text());
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Output choice error: " << std::string(e.what()) << std::endl;
        return;
    }

    // Delete the previous output options.
    output->clear();
    // Rebuild the device options.
    buildDevices();

}

void AudioSettingsDialog::save()
{
    // Save the new settings in the config file.
    AppConfig& config = loadConfig();
    config.audio.backend = backend->text();
    config.audio.outputDevice = output->text();
    config.audio.inputDevice = input->text();
    saveConfig();
}

void AudioSettingsDialog::cancel()
{
    // The backend or devices have been changed.
    // Reset it all to the initial settings set in the config file.
    if (backend->changed() || output->changed()) {
        try {
            application.getAudioEngine().setBackend(backend->text());
            application.getAudioEngine().setOutputDevice(output->text());
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Backend choice error: " << std::string(e.what()) << std::endl;
            return;
        }

        // Delete the previous device options.
        output->clear();
        input->clear();
        // Rebuild the device options.
        buildDevices();
    }
}

