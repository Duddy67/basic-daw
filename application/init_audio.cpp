#include "../main.h"

void Application::initAudioBackend()
{
    auto backends = getAudioEngine().getBackends();
    auto config = loadConfig(CONFIG_FILENAME);
    unsigned int index = 0;

    // Check for first starting.
    if (config.backend == "") {
        // Search for JACK or the backend set by the system by default.
        // If nothing found, the first backend on the list will be used (ie: index 0).
        for (size_t i = 0; i < backends.size(); ++i) {
            // If JACK is available, use it.
            if (backends[i].name == "JACK") {
                index = i;
                break;
            }

            if (backends[i].isDefault) {
                index = i;
            }
        }

        // Update setting.
        config.backend = backends[index].name;
        saveConfig(config, CONFIG_FILENAME);
    }

    // Initialize backend.
    getAudioEngine().setBackend(config.backend.c_str());
}

void Application::initAudioDevices()
{
    auto config = loadConfig(CONFIG_FILENAME);
    unsigned int index = 0;

    // Check for first starting.
    if (config.outputDevice == "") {
        // Privilege duplex devices if available.
        auto duplexDevices = getAudioEngine().getDuplexDevices();

        if (duplexDevices.size() != 0) {
            // Search for the device set by the system by default.
            // If no found, the first device on the list (ie: index 0) will be used.
            for (size_t i = 0; i < duplexDevices.size(); ++i) {
                if (duplexDevices[i].isDefault) {
                    index = i;
                }
            }

            config.outputDevice = duplexDevices[index].name;
            config.inputDevice = duplexDevices[index].name;
        }
        else {
            auto outputDevices = getAudioEngine().getOutputDevices();
            for (size_t i = 0; i < outputDevices.size(); ++i) {
                if (outputDevices[i].isDefault) {
                    index = i;
                }
            }

            config.outputDevice = outputDevices[index].name;

            auto inputDevices = getAudioEngine().getInputDevices();
            index = 0;
            for (size_t i = 0; i < inputDevices.size(); ++i) {
                if (inputDevices[i].isDefault) {
                    index = i;
                }
            }

            config.inputDevice = inputDevices[index].name;
        }

        saveConfig(config, CONFIG_FILENAME);
    }

    // Check first if the selected device is duplex. 
    if (config.outputDevice.compare(config.inputDevice) == 0 && getAudioEngine().isDeviceDuplex(config.outputDevice.c_str())) {
        getAudioEngine().setDuplexDevice(config.outputDevice.c_str());
        getAudioEngine().startDuplex();
    }
    // If no duplex device, fall back on standard output/input devices.
    else {
        getAudioEngine().setOutputDevice(config.outputDevice.c_str());
        getAudioEngine().startPlayback();
        getAudioEngine().setInputDevice(config.inputDevice.c_str());
        getAudioEngine().startCapture();
    }
}

void Application::initAudioSystem()
{
    // Create and initialize the audio engine object.
    audioEngine = new Audio::Engine(*this);

    try {
        initAudioBackend();
        std::cout << "Current backend: " << audioEngine->currentBackend() << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Backend error: " << std::string(e.what()) << std::endl;
        return;
    }

    // Get the initialized config file.
    auto config = loadConfig(CONFIG_FILENAME);

    // Initialize devices.
    try {
        initAudioDevices();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Device error: " << std::string(e.what()) << std::endl;
        return;
    }

    // Set sample rate for time computing.
    //time->setSampleRate(audioEngine->getDefaultOutputSampleRate());

    //audioEngine->printAllDevices(); // For debug purpose.
    std::cout << "=== Audio system initialized ===" << std::endl;
}

