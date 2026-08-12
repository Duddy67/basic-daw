#include "track.h"


namespace Midi {

    Track::Track(Engine& e, int id) : Core::Track(id), engine(e)
    {
        // ...
        fillEventList();
    }

    Track::~Track()
    {
        // ...
    }

    /*void Track::processMessage(const std::vector<unsigned char>& message, double deltaTime) 
    {
        // First, check for solo tracks.
        if (engine.getProject()->soloTrackCount() && !isSoloed()) {
            // This track is not part of the currently soloed tracks.
            // Don't play.
            return;
        }

        if (!engine.getMidiOut().isPortOpen()) {
            std::cout << "MIDI output port is not open!" << std::endl;
            return;
        }

        if (!isMuted()) {
            unsigned int nBytes = message.size();
            for (unsigned int i = 0; i < nBytes; i++) {
                std::cout << "Byte " << i << " = " << (int)message.at(i) << ", ";
            }

            if (nBytes > 0) {
                std::cout << "stamp = " << deltaTime << ", ";
            }

            engine.getMidiOut().sendMessage(&message);
        }

        auto& transport = engine.getApplication().getTransport();

        // Store incoming midi events.
        if (transport.isRecording()) {
            MidiEvent event;
            event.samplePosition = transport.getPlayheadSample();
            event.message = message;
            events.push_back(event);
        }
    }*/

    void Track::addEvent(MidiEvent& event)
    {
        events.push_back(event);
    }

    /*
     * FOR TEST PURPOSE ONLY
     */
    void Track::fillEventList()
    {
        std::vector<unsigned char> noteOn = {0x90, 53, 80}; // TEST MESSAGE
        MidiEvent event1;
        event1.samplePosition = 1000;
        event1.message = noteOn;
        events.push_back(event1);

        std::vector<unsigned char> noteOff = {0x80, 53, 0}; // TEST MESSAGE
        MidiEvent event2;
        event2.samplePosition = 15000;
        event2.message = noteOff;
        events.push_back(event2);

        std::vector<unsigned char> noteOn2 = {0x90, 63, 90}; // TEST MESSAGE
        MidiEvent event3;
        event3.samplePosition = 30000;
        event3.message = noteOn2;
        events.push_back(event3);

        std::vector<unsigned char> noteOff2 = {0x80, 63, 0}; // TEST MESSAGE
        MidiEvent event4;
        event4.samplePosition = 45000;
        event4.message = noteOff2;
        events.push_back(event4);
    }
}



