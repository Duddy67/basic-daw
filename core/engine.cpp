#include "engine.h"
#include <iostream>
#include "../main.h"

namespace Core {

    Engine::~Engine() {
        // Clears all audio ressources currently used by JACK. 
        shutdownJack();
    }

    bool Engine::initializeJack()
    {
        // Open client.
        jack_status_t status;
        client = jack_client_open("audiocute", JackNullOption, &status, nullptr);

        if (!client) {
            std::cerr << "Cannot open JACK client (status = " << status << ")" << std::endl;
            return false;
        }

        jack_nframes_t sampleRate = jack_get_sample_rate(client);

        if (sampleRate != defaultOutputSampleRate) {
            std::cout << "JACK sample rate is " << sampleRate << " Hz. Adjusting internally.\n";
        }

        // Register application's audio ports.
        audioOutputs[0] = jack_port_register(client, "out_left", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
        audioOutputs[1] = jack_port_register(client, "out_right", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
        audioInputs[0] = jack_port_register(client, "in_left", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
        audioInputs[1] = jack_port_register(client, "in_right", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

        // Register application's midi ports.
        midiOutput = jack_port_register(client, "midi_out", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
        midiInput = jack_port_register(client, "midi_in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);

        if (!audioOutputs[0] || !audioOutputs[1] || !audioInputs[0] || !audioInputs[1] || !midiOutput || !midiInput) {
            std::cerr << "Failed to register one or more JACK ports.\n";
            return false;
        }

        // Set the process callback function.
        jack_set_process_callback(client, jack_process_callback, this);

        // Activate the client.
        if (jack_activate(client)) {
            std::cerr << "Cannot activate JACK client.\n";
            return false;
        }

        std::cout << "JACK client initialized successfully." << std::endl;
        return true;
    }

    /*
     * Closes JACK client.
     */
    void Engine::shutdownJack()
    {
        if (client) {
            jack_deactivate(client);
            jack_client_close(client);
            client = nullptr;
        }
    }

    /*
     * Callback function used by JACK to feed audio and midi data to ports.
     */
    int Engine::jack_process_callback(jack_nframes_t nframes, void* arg)
    {
        Engine* self = static_cast<Engine*>(arg);

        // Check first that a project exists.
        if (!self->application.getProject()) {
            // JACK expects zero on success.
            return 0;
        }

        
        // --- Get audio buffers ---
        float* outL = (float*)jack_port_get_buffer(self->audioOutputs[0], nframes);
        float* outR = (float*)jack_port_get_buffer(self->audioOutputs[1], nframes);
        float* inL  = (float*)jack_port_get_buffer(self->audioInputs[0], nframes);
        float* inR  = (float*)jack_port_get_buffer(self->audioInputs[1], nframes);

        // --- Get MIDI buffers ---
        void* midiInBuffer  = jack_port_get_buffer(self->midiInput, nframes);
        void* midiOutBuffer = jack_port_get_buffer(self->midiOutput, nframes);

        // Clear MIDI output.
        jack_midi_clear_buffer(midiOutBuffer);

        // Get current sample position.
        uint64_t currentSample = self->application.getTransport().getPlayheadSample();

        // --- Process MIDI ---
        // Write events to midi out buffer with sample offset.
        self->application.getMidiScheduler().processOutput(nframes, midiOutBuffer, currentSample);
        // 
        self->application.getMidiScheduler().processInput(midiInBuffer, currentSample);

        // --- Process audio (playback and capture) ---
        self->application.getAudioProcessor().process(nframes, outL, outR, inL, inR);

        // Advance playhead for next cycle.
        self->application.getTransport().advancePlayhead(nframes);

        // JACK expects zero on success.
        return 0;
    }

    /*
     * Connects the application's ports to the corresponding and available JACK's audio ports.
     */
    void Engine::connectAudioPorts()
    {
        // Make sure the client is activated.
        if (!client) {
            return;
        }

        // System playback ports appear as input ports from a JACK's perspective.
        const char** physicalPlayback = jack_get_ports(client, NULL, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput | JackPortIsPhysical);
        auto config = loadConfig();
        int i = 0, idL = -1, idR = -1;

        if (physicalPlayback) {
            // Get the application's output port names.
            const char* outLName = jack_port_name(audioOutputs[0]);
            const char* outRName = jack_port_name(audioOutputs[1]);

            // Search for the output ports set in the config file.
            while (physicalPlayback[i]) {
                // Compare port names.
                if (strcmp(physicalPlayback[i], config.audio.outputLeft.c_str()) == 0) {
                    //std::cout << "FOUND: " << physicalPlayback[i] << std::endl;
                    idL = i;
                }

                if (strcmp(physicalPlayback[i], config.audio.outputRight.c_str()) == 0) {
                    //std::cout << "FOUND: " << physicalPlayback[i] << std::endl;
                    idR = i;
                }

                i++;
            }

            // Set ids to default (ie: 0 and 1) in case no output port
            // has been found in the config file.
            idL = idL < 0 ? 0 : idL;
            idR = idR < 0 ? 1 : idR;

            // Connect left channel to physical playback port 1.
            if (physicalPlayback[idL]) {
                safeConnect(outLName, physicalPlayback[idL]);
                config.audio.outputLeft = physicalPlayback[idL];
            }

            // Connect right channel to physical playback port 2.
            if (physicalPlayback[idR]) {
                safeConnect(outRName, physicalPlayback[idR]);
                config.audio.outputRight = physicalPlayback[idR];
            }

            std::cout << "Connected outputs to: " << (physicalPlayback[idL] ?  physicalPlayback[idL] : "(none)") << " and "
                      << (physicalPlayback[idR] ? physicalPlayback[idR] : "(none)") << std::endl;

            jack_free(physicalPlayback);
        }
        else {
            config.audio.outputLeft = "";
            config.audio.outputRight = "";
            std::cerr << "No physical playback ports found (Input flag)." << std::endl;
        }

        // System capture ports appear as output ports from a JACK's perspective.
        const char** physicalCapture = jack_get_ports(client, NULL, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput | JackPortIsPhysical);
        // Reinitialize ids.
        i = 0;
        idL = -1;
        idR = -1;

        if (physicalCapture) {
            // Get the application's input port names.
            const char* inLName = jack_port_name(audioInputs[0]);
            const char* inRName = jack_port_name(audioInputs[1]);

            // Search for the input ports set in the config file.
            while (physicalCapture[i]) {
                // Compare port names.
                if (strcmp(physicalCapture[i], config.audio.inputLeft.c_str()) == 0) {
                    //std::cout << "FOUND: " << physicalCapture[i] << std::endl;
                    idL = i;
                }

                if (strcmp(physicalCapture[i], config.audio.inputRight.c_str()) == 0) {
                    //std::cout << "FOUND: " << physicalCapture[i] << std::endl;
                    idR = i;
                }

                i++;
            }

            // Set ids to default (ie: 0 and 1) in case no input port
            // has been found in the config file.
            idL = idL < 0 ? 0 : idL;
            idR = idR < 0 ? 1 : idR;

            // Connect left channel to physical capture port 1.
            if (physicalCapture[idL]) {
                safeConnect(physicalCapture[idL], inLName);
                config.audio.inputLeft = physicalCapture[idL];
            }

            // Connect right channel to physical capture port 2.
            if (physicalCapture[idR]) {
                safeConnect(physicalCapture[idR], inRName);
                config.audio.inputRight = physicalCapture[idR];
            }

            std::cout << "Connected inputs to: " << (physicalCapture[idL] ?  physicalCapture[idL] : "(none)") << " and "
                      << (physicalCapture[idR] ? physicalCapture[idR] : "(none)") << std::endl;

            jack_free(physicalCapture);
        }
        else {
            config.audio.inputLeft = "";
            config.audio.inputRight = "";
            std::cerr << "No physical capture ports found (Output flag)." << std::endl;
        }

        saveConfig();
    }

    /*
     * Connects the application's ports to the corresponding and available JACK's midi ports.
     */
    void Engine::connectMidiPorts()
    {
        // Make sure the client is activated.
        if (!client) {
            return;
        }

        // System playback ports appear as input ports from a JACK's perspective.
        const char** physicalPlayback = jack_get_ports(client, NULL, JACK_DEFAULT_MIDI_TYPE, JackPortIsInput | JackPortIsPhysical);
        auto config = loadConfig();
        int i = 0, idPort = -1;

        if (physicalPlayback) {
            // Get the application's midi output port name.
            const char* midiOutName = jack_port_name(midiOutput);

            // Search for the output ports set in the config file.
            while (physicalPlayback[i]) {
                // Compare port names.
                if (strcmp(physicalPlayback[i], config.midi.outputPort.c_str()) == 0) {
                    //std::cout << "FOUND: " << physicalPlayback[i] << std::endl;
                    idPort = i;
                }

                i++;
            }

            // Set ids to default (ie: 0 and 1) in case no output port
            // has been found in the config file.
            idPort = idPort < 0 ? 0 : idPort;

            // Connect midi output to physical playback port 1.
            if (physicalPlayback[idPort]) {
                safeConnect(midiOutName, physicalPlayback[idPort]);
                config.midi.outputPort = physicalPlayback[idPort];
            }

            std::cout << "Connected MIDI output to: " << (physicalPlayback[idPort] ? physicalPlayback[idPort] : "(none)") << std::endl;

            jack_free(physicalPlayback);
        }
        else {
            config.midi.outputPort = "";
            std::cerr << "No physical playback MIDI port found (Input flag)." << std::endl;
        }

        // System capture midi ports appear as output ports from a JACK's perspective.
        const char** physicalCapture = jack_get_ports(client, NULL, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput | JackPortIsPhysical);
        // Reinitialize ids.
        i = 0;
        idPort = -1;

        if (physicalCapture) {
            // Get the application's midi input port name.
            const char* midiInName = jack_port_name(midiInput);

            // Search for the input ports set in the config file.
            while (physicalCapture[i]) {
                // Compare port names.
                if (strcmp(physicalCapture[i], config.midi.inputPort.c_str()) == 0) {
                    //std::cout << "FOUND: " << physicalCapture[i] << std::endl;
                    idPort = i;
                }

                i++;
            }

            // Set ids to default (ie: 0 and 1) in case no input port
            // has been found in the config file.
            idPort = idPort < 0 ? 0 : idPort;

            // Connect midi input to physical capture port 1.
            if (physicalCapture[idPort]) {
                safeConnect(physicalCapture[idPort], midiInName);
                config.midi.inputPort = physicalCapture[idPort];
            }

            std::cout << "Connected MIDI input to: " << (physicalCapture[idPort] ? physicalCapture[idPort] : "(none)") << std::endl;

            jack_free(physicalCapture);
        }
        else {
            config.midi.inputPort = "";
            std::cerr << "No physical capture MIDI port found (Output flag)." << std::endl;
        }

        saveConfig();
    }

    void Engine::safeConnect(const char* src, const char* dest)
    {
        // Make sure the client is activated.
        if (!client) {
            return;
        }

        int result = jack_connect(client, src, dest);

        if (result != 0 && result != EEXIST) {
            std::cout << "Failed to connect: " << src << " -> " << dest << " (error " << result << ")" << std::endl;
        }
    }

    void Engine::safeDisconnect(const char* src, const char* dest)
    {
        // Make sure the client is activated.
        if (!client) {
            return;
        }

        int result = jack_disconnect(client, src, dest);

        if (result != 0 && result != EEXIST) {
            std::cout << "Failed to disconnect: " << src << " -> " << dest << " (error " << result << ")" << std::endl;
        }
    }

    std::vector<std::string> Engine::getPorts(DataType dataType, ConnectionType connectionType)
    {
        std::vector<std::string> portList;

        // Make sure the client is initialized.
        if (!client) {
            return portList;
        }

        // Set the flags parameter according to the type of port names to return.
        // Note: System playback ports appear as input ports from a JACK's perspective.
        //       System capture ports appear as output ports from a JACK's perspective.
        unsigned long flags = (connectionType == ConnectionType::INPUT) ? JackPortIsOutput : JackPortIsInput;
        const char* portNamePattern = (dataType == DataType::AUDIO) ? JACK_DEFAULT_AUDIO_TYPE : JACK_DEFAULT_MIDI_TYPE;

        const char** ports = jack_get_ports(client, NULL, portNamePattern, JackPortIsPhysical | flags);

        if (ports == NULL) {
            // No port found or an error occured;
            return portList;
        }

        // Iterate through the NULL terminated list and add each port name.
        for (int i = 0; ports[i] != NULL; i++) {
            portList.push_back(std::string(ports[i]));
        }

        // Free the memory allocated by JACK.
        jack_free(ports);

        return portList;
    }

    std::vector<std::string> Engine::getOutputPorts(DataType dataType)
    {
        return getPorts(dataType, ConnectionType::OUTPUT);
    }

    std::vector<std::string> Engine::getInputPorts(DataType dataType)
    {
        return getPorts(dataType, ConnectionType::INPUT);
    }

    /*
     * Returns the application's output port name according to the given direction.
     */
    const char* Engine::getOutputPortName(Direction direction)
    {
        return direction == Direction::LEFT ? jack_port_name(audioOutputs[0]) : jack_port_name(audioOutputs[1]);
    }

    /*
     * Returns the application's input port name according to the given direction.
     */
    const char* Engine::getInputPortName(Direction direction)
    {
        return direction == Direction::LEFT ? jack_port_name(audioInputs[0]) : jack_port_name(audioInputs[1]);
    }

    const char* Engine::getAudioPortName(ConnectionType connectionType, Direction direction)
    {
        if (connectionType == ConnectionType::OUTPUT) {
            return direction == Direction::LEFT ? jack_port_name(audioOutputs[0]) : jack_port_name(audioOutputs[1]);
        }
        else {
            return direction == Direction::LEFT ? jack_port_name(audioInputs[0]) : jack_port_name(audioInputs[1]);
        }
    }

    /*
     * Returns the application's MIDI port name according to the given direction.
     */
    const char* Engine::getMidiPortName(ConnectionType connectionType)
    {
        return connectionType == ConnectionType::INPUT ? jack_port_name(midiInput) : jack_port_name(midiOutput);
    }

    /*
     * Displays both the input and output available audio and midi ports in the console.
     * Function used for debugging purpose.
     */
    void Engine::printAllPorts()
    {
        // Make sure the client is initialized.
        if (!client) {
            return;
        }

        auto audioOutputs = getOutputPorts(DataType::AUDIO);
        auto audioInputs = getInputPorts(DataType::AUDIO);

        std::cout << std::endl;
        std::cout << "=== Available Audio Ports ===" << std::endl;

        std::cout << "\nOutput Ports:" << std::endl;
        for (size_t i = 0; i < audioOutputs.size(); ++i) {
            std::cout << "  " << i + 1 << ": " << audioOutputs[i];
            std::cout << std::endl;
        }

        std::cout << "\nInput Ports:" << std::endl;
        for (size_t i = 0; i < audioInputs.size(); ++i) {
            std::cout << "  " << i + 1 << ": " << audioInputs[i];
            std::cout << std::endl;
        }

        auto midiOutputs = getOutputPorts(DataType::MIDI);
        auto midiInputs = getInputPorts(DataType::MIDI);

        std::cout << std::endl;
        std::cout << "=== Available MIDI Ports ===" << std::endl;

        std::cout << "\nOutput Ports:" << std::endl;
        for (size_t i = 0; i < midiOutputs.size(); ++i) {
            std::cout << "  " << i + 1 << ": " << midiOutputs[i];
            std::cout << std::endl;
        }

        std::cout << "\nInput Ports:" << std::endl;
        for (size_t i = 0; i < midiInputs.size(); ++i) {
            std::cout << "  " << i + 1 << ": " << midiInputs[i];
            std::cout << std::endl;
        }
    }
}
