#include "engine.h"
#include "../project/model.h"
//#include "RtMidi.h"
#include "track.h"


namespace Midi {
    Engine::Engine(Application& app) : application(app)
    {
        // Create an api map.
        //apiMap[RtMidi::UNIX_JACK] = "Jack Client";
        //apiMap[RtMidi::LINUX_ALSA] = "Linux ALSA";
        //apiMap[RtMidi::RTMIDI_DUMMY] = "RtMidi Dummy";

        // Set the apis vector according to the available devices on the system.
        //RtMidi::getCompiledApi(apis);
    }

    Engine::~Engine()
    {
        //deleteCurrentPorts();
    }

    Project::Model* Engine::getProject()
    {
        return application.getProject();
    }

    /*void Engine::deleteCurrentPorts()
    {
        // Free memory.
        delete midiIn;
        delete midiOut;
        // Prevent any dangling pointers.
        midiIn = nullptr;
        midiOut = nullptr;
    }

    void Engine::initDevice(const char* name)
    {
        deleteCurrentPorts();
        int deviceId = -1;
        AppConfig& config = loadConfig();

        // Try first with the possible given name.
        if (strcmp(name, "none") != 0) {
            for (unsigned int i = 0; i < apis.size(); i++) {
                if (strcmp(apiMap[apis[i]].c_str(), name) == 0) {
                    deviceId = i;
                    break;
                }
            }
        }

        // Check for default device in the config.
        if (deviceId == -1) {
            for (unsigned int i = 0; i < apis.size(); i++) {
                // If no device is set by default and Jack is available, use it.
                if (config.midi.device == "" && apiMap[apis[i]] == "Jack Client") {
                    deviceId = i;
                    break;
                }

                // Set the default device, if any.
                if (apiMap[apis[i]] == config.midi.device) {
                    deviceId = i;
                    break;
                }
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

            std::cout << "RtMidi API: " << apiMap[apis[deviceId]] << std::endl;
        }
        else {
            throw std::runtime_error("No MIDI device available on the system.");
        }
    }

    void Engine::initInputPort(const char* name)
    {
        int portId = -1;
        AppConfig& config = loadConfig();

        // Try first with the possible given name.
        if (strcmp(name, "none") != 0) {
            for (unsigned int i = 0; i < midiIn->getPortCount(); i++) {
                if (strcmp(midiIn->getPortName(i).c_str(), name) == 0) {
                    portId = i;
                    break;
                }
            }
        }

        // Check for default input port in the config.
        if (portId == -1) {
            for (unsigned int i = 0; i < midiIn->getPortCount(); i++) {
                if (midiIn->getPortName(i) == config.midi.inputPort) {
                    portId = i;
                }
            }
        }

        // In case no port has been set as default so far, open the first available port.
        portId = (portId == -1 && midiIn->getPortCount()) ? 0 : portId;

        if (portId > -1) {
            midiIn->openPort(portId);
            // Set the callback function immediately to avoid 
            // having incoming messages written to the queue.
            midiIn->setCallback(&data_callback, this);
            // Set or update the midi config.
            config.midi.inputPort = midiIn->getPortName(portId);
            saveConfig();
        }
        else {
            throw std::runtime_error("No MIDI input port available on the system.");
        }
    }

    void Engine::initOutputPort(const char* name)
    {
        int portId = -1;
        AppConfig& config = loadConfig();

        // Try first with the possible given name.
        if (strcmp(name, "none") != 0) {
            for (unsigned int i = 0; i < midiOut->getPortCount(); i++) {
                if (strcmp(midiOut->getPortName(i).c_str(), name) == 0) {
                    portId = i;
                    break;
                }
            }
        }

        if (portId == -1) {
            for (unsigned int i = 0; i < midiOut->getPortCount(); i++) {
                if (midiOut->getPortName(i) == config.midi.outputPort) {
                    portId = i;
                }
            }
        }

        // In case no port has been set as default so far, open the first available port.
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

    void Engine::data_callback(double deltaTime, std::vector<unsigned char>* message, void* userData)
    {
        Midi::Engine* engine = (Midi::Engine*) userData;

        // Make sure a project does exist.
        if (engine->application.getProject() != nullptr) {
            // Send the incoming message to the existing tracks.
            for (auto& track : engine->application.getProject()->getTracks()) {
                // Only MIDI tracks process MIDI messages.
                if (track->getType() == DataType::MIDI) {
                    auto* midiTrack = static_cast<Midi::Track*>(track.get());
                    midiTrack->processMessage(*message, deltaTime);
                }
            }
        }
    }*/

    void Engine::process(jack_nframes_t nframes, void* midiInputBuffer, void* midiOutputBuffer)
    {
        // Make sure a project does exist.
        if (application.getProject() != nullptr) {
            // Send the incoming message to the existing tracks.
            for (auto& track : application.getProject()->getTracks()) {
                // Only MIDI tracks process MIDI messages.
                if (track->getType() == DataType::MIDI) {
                    //auto* midiTrack = static_cast<Midi::Track*>(track.get());
                    //midiTrack->processMessage(*message, deltaTime);
                }
            }
        }
    }
}

