#include "track.h"


namespace Midi {

    Track::Track(int id) : Core::Track(id)
    {
        // ...
        fillEventList();
    }

    Track::~Track()
    {
        // ...
    }

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



