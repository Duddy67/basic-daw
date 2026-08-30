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
    locateToSample(0); 
    auto& tracks = application.getProject()->getTracks();

    for (auto& track : tracks) {
        if (track->getType() == DataType::MIDI) {
            auto* midiTrack = static_cast<Midi::Track*>(track.get());
            midiTrack->playbackCursor = 0;
        }
    }
    ///// END TEMPORARY  //////

    application.getProjectView().startLiveUpdate();
    playing.store(true);
}

void Transport::stop()
{
    playing.store(false);
    recording.store(false);
    application.getProjectView().stopLiveUpdate();
}

void Transport::record()
{
    playing.store(false);
    recording.store(true);
}
