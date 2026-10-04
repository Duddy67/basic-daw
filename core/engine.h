#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <jack/jack.h>
#include <jack/midiport.h>
#include <iostream>
#include <vector>
#include <memory>
#include <atomic>
#include "sndfile.h"
#include "midi_event_queue.h"
#include "../constants.h"

class Application;

/*
 * Class in charge of setting up and initialing the JACK client.
 */
namespace Core {

    class Engine {
            Application& application;
            jack_client_t* client = nullptr;
            jack_port_t* audioOutputs[2] = {nullptr, nullptr}; // Left, Right
            jack_port_t* audioInputs[2] = {nullptr, nullptr}; // Left, Right
            jack_port_t* midiOutput = nullptr;
            jack_port_t* midiInput = nullptr;
            std::atomic<bool> playbackActive{false};
            std::atomic<bool> captureActive{false};
            int defaultOutputFormat = SF_FORMAT_FLOAT;
            const uint32_t sampleRate = 44100;
            MidiEventQueue uiToAudioQueue;

            static int jack_process_callback(jack_nframes_t nframes, void* arg);
            void processMidiInput(void* midiInBuffer, void* midiOutBuffer, jack_nframes_t nframes);
            std::vector<std::string> getPorts(DataType dataType, ConnectionType connectionType);

        public:

            Engine(Application& app) : application(app) {}
            ~Engine();

            bool initializeJack();
            bool isClientActivated() { return client ? true : false; }
            void shutdownJack();
            void safeConnect(const char* src, const char* dest);
            void safeDisconnect(const char* src, const char* dest);
            void startPlayback() { playbackActive = true; }
            void stopPlayback()  { playbackActive = false; }
            void startCapture()  { captureActive = true; }
            void stopCapture()   { captureActive = false; }
            void startDuplex()   { playbackActive = true; captureActive = true; }
            void stopDuplex()    { playbackActive = false; captureActive = false; }
            void connectAudioPorts();
            void connectMidiPorts();
            void printAllPorts();
            std::vector<std::string> getOutputPorts(DataType type);
            std::vector<std::string> getInputPorts(DataType type);
            const char* getOutputPortName(Direction direction);
            const char* getInputPortName(Direction direction);
            const char* getAudioPortName(ConnectionType connectionType, Direction direction);
            const char* getMidiPortName(ConnectionType connectionType);
            int getDefaultOutputFormat() const { return defaultOutputFormat; }
            uint32_t getSampleRate() const { return sampleRate; }
            Application& getApplication() const { return application; }
            bool pushUiMidiEvent(const QueuedMidiEvent& event) { return uiToAudioQueue.push(event); }
            void drainUiMidiQueue(void* midiOutBuffer);
    };
}

#endif // CORE_ENGINE_H
