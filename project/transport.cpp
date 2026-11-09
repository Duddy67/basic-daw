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

void Transport::locateToSample(uint64_t sample)
{
    // Update the playhead position.
    playheadSample.store(sample, std::memory_order_release);
    // Update the playhead position in MIDI tracks.
    application.getMidiScheduler().relocate(sample);
}

void Transport::play() {
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
