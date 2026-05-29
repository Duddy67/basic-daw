#ifndef MIDI_ENGINE_H
#define MIDI_ENGINE_H

#include <vector>
#include <map>
#include "../main.h"
#include "RtMidi.h"


class Application;

namespace Midi {

    class Engine {
        Application& application;
        RtMidiIn* midiIn = nullptr;
        RtMidiOut* midiOut = nullptr;
        std::map<int, std::string> apiMap;
        std::vector<RtMidi::Api> apis;

        public:

            Engine(Application& app);
            ~Engine();

            size_t getDeviceCount() { return apis.size(); }
            void initMidiDevice(AppConfig& config);
            void initMidiPorts(AppConfig& config);
            RtMidiIn& getMidiIn() { return *midiIn; }
            RtMidiOut& getMidiOut() { return *midiOut; }
            int getInputPortCount() { return midiIn ? midiIn->getPortCount() : 0; }
            int getOutputPortCount() { return midiOut ? midiOut->getPortCount() : 0; }
            std::vector<std::string> getDevices();
            std::vector<std::string> getInputPorts();
            std::vector<std::string> getOutputPorts();
    };
}


#endif // MIDI_ENGINE_H
