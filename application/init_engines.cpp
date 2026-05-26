#include "../main.h"

void Application::initAudioBackend()
{
    auto backends = getAudioEngine().getBackends();
    AppConfig& config = loadConfig();
    unsigned int index = 0;

    // Check for first starting.
    if (config.audio.backend == "") {
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
        config.audio.backend = backends[index].name;
        saveConfig();
    }

    // Initialize backend.
    getAudioEngine().setBackend(config.audio.backend.c_str());
}

void Application::initAudioDevices()
{
    AppConfig& config = loadConfig();
    unsigned int index = 0;

    // Check for first starting.
    if (config.audio.outputDevice == "") {
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

            config.audio.outputDevice = duplexDevices[index].name;
            config.audio.inputDevice = duplexDevices[index].name;
        }
        else {
            auto outputDevices = getAudioEngine().getOutputDevices();
            for (size_t i = 0; i < outputDevices.size(); ++i) {
                if (outputDevices[i].isDefault) {
                    index = i;
                }
            }

            config.audio.outputDevice = outputDevices[index].name;

            auto inputDevices = getAudioEngine().getInputDevices();
            index = 0;
            for (size_t i = 0; i < inputDevices.size(); ++i) {
                if (inputDevices[i].isDefault) {
                    index = i;
                }
            }

            config.audio.inputDevice = inputDevices[index].name;
        }

        saveConfig();
    }

    // Check first if the selected device is duplex. 
    if (config.audio.outputDevice.compare(config.audio.inputDevice) == 0 && getAudioEngine().isDeviceDuplex(config.audio.outputDevice.c_str())) {
        getAudioEngine().setDuplexDevice(config.audio.outputDevice.c_str());
        getAudioEngine().startDuplex();
    }
    // If no duplex device, fall back on standard output/input devices.
    else {
        getAudioEngine().setOutputDevice(config.audio.outputDevice.c_str());
        getAudioEngine().startPlayback();
        getAudioEngine().setInputDevice(config.audio.inputDevice.c_str());
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

void Application::initMidiSystem()
{
    try {
        // Create and initialize the midi engine object.
        midiEngine = new Midi::Engine(*this);
    }
    catch (RtMidiError& e) {
        std::cerr << "RtMidi error: " << std::string(e.what()) << std::endl;
        return;
    }
}

