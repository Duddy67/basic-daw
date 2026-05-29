#include "engine.h"


namespace Midi {
    Engine::Engine(Application& app) : application(app)
    {
        // Create an api map.
        apiMap[RtMidi::UNIX_JACK] = "Jack Client";
        apiMap[RtMidi::LINUX_ALSA] = "Linux ALSA";
        apiMap[RtMidi::RTMIDI_DUMMY] = "RtMidi Dummy";

        // Set the apis vector according to the available devices on the system.
        RtMidi::getCompiledApi(apis);
    }

    Engine::~Engine()
    {
        delete midiIn;
        delete midiOut;
    }

    void Engine::initMidiDevice(AppConfig& config)
    {
        int deviceId = -1;

        // Check for default device in the config.
        for (unsigned int i = 0; i < apis.size(); i++) {
            if (apiMap[apis[i]] == config.midi.device) {
                deviceId = i;
                break;
            }

            // If no device is set by default, use Jack.
            if (config.midi.device == "" && apiMap[apis[i]] == "Jack Client") {
                deviceId = i;
                break;
            }
        }  

        // Jack is not available and no default device has been set. 
        // Use the first device on the list.
        deviceId = (deviceId == -1 && apis.size()) ? 0 : deviceId;

        if (deviceId > -1) {
            // RtMidi constructors (exception possible, let propagate).
            midiIn = new RtMidiIn(apis[deviceId]);          
            midiOut = new RtMidiOut(apis[deviceId]);          

            config.midi.device = apiMap[apis[deviceId]];
            saveConfig();

            std::cout << "API: " << apiMap[apis[deviceId]] << std::endl;
        }
        else {
            throw std::runtime_error("No MIDI device available on the system.");
        }
    }

    void Engine::initMidiPorts(AppConfig& config)
    {
        int portId = -1;

        // Check for default input port in the config.
        for (unsigned int i = 0; i < midiIn->getPortCount(); i++) {
            if (midiIn->getPortName(i) == config.midi.inputPort) {
                portId = i;
            }
            std::cout << "Input port: " << midiIn->getPortName(i) << std::endl;
        }

        // In case no port is set as default, open the first available port.
        portId = (portId == -1 && midiIn->getPortCount()) ? 0 : portId;

        if (portId > -1) {
            midiIn->openPort(portId);
            // Update the midi config.
            config.midi.inputPort = midiIn->getPortName(portId);
            saveConfig();
        }
        else {
            throw std::runtime_error("No MIDI input port available on the system.");
        }

        // Same for output port.

        portId = -1;

        for (unsigned int i = 0; i < midiOut->getPortCount(); i++) {
            if (midiOut->getPortName(i) == config.midi.outputPort) {
                portId = i;
            }
        }

        portId = (portId == -1 && midiOut->getPortCount()) ? 0 : portId;

        if (portId > -1) {
            midiOut->openPort(portId);
            config.midi.outputPort = midiOut->getPortName(portId);
            saveConfig();
        }
        else {
            throw std::runtime_error("No MIDI output port available on the system.");
        }
    }

    std::vector<std::string> Engine::getDevices()
    {
        std::vector<std::string> devices;

        for (unsigned int i = 0; i < apis.size(); i++) {
            devices.push_back(apiMap[apis[i]]);
        }

        return devices;
    }

    std::vector<std::string> Engine::getInputPorts()
    {
        std::vector<std::string> inputs;

        for (unsigned int i = 0; i < midiIn->getPortCount(); i++) {
            inputs.push_back(midiIn->getPortName(i));
        }

        return inputs;
    }

    std::vector<std::string> Engine::getOutputPorts()
    {
        std::vector<std::string> outputs;

        for (unsigned int i = 0; i < midiOut->getPortCount(); i++) {
            outputs.push_back(midiOut->getPortName(i));
        }

        return outputs;
    }
}

