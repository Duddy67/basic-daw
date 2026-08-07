#include "transport.h"
#include "../midi/track.h"


Transport::Transport(Application& app) : application(app)
{
    // ...
}

Transport::~Transport()
{
    // ...
}

void Transport::play() {
    // TEMPORARY (starts from the top each time)
    playheadSample.store(0); 
    auto& tracks = application.getProject()->getTracks();

    for (auto& track : tracks) {
        if (track->getType() == DataType::MIDI) {
            auto* midiTrack = static_cast<Midi::Track*>(track.get());
            midiTrack->playbackCursor = 0;
        }
    }
    ///// END TEMPORARY  //////

    std::cout << " Play playheadSample: " << playheadSample.load() << std::endl;
    playing.store(true);
}

void Transport::stop()
{
    playing.store(false);
    recording.store(false);
}

void Transport::record()
{
    playing.store(false);
    recording.store(true);
}
