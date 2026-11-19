#include "transport.h"
#include "../midi/track.h"


Transport::Transport()
{
    // ...
}

Transport::~Transport()
{
    // ...
}

/*
 * Setter used to register callback functions.
 */
void Transport::setPlayCallbacks(std::function<void()> start, std::function<void()> stop)
{
    // Registers callback functions.
    onPlayStarted = start;
    onPlayStopped = stop;
}

void Transport::play() {
    playing.store(true);

    // Is there a callback registered?
    if (onPlayStarted) {
        onPlayStarted();
    }
}

void Transport::stop()
{
    playing.store(false);
    recording.store(false);

    // Is there a callback registered?
    if (onPlayStopped) {
        onPlayStopped();
    }
}

void Transport::record()
{
    playing.store(false);
    recording.store(true);
}
