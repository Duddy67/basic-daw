#include "transport.h"


Transport::Transport(Application& app) : application(app)
{
    // ...
}

Transport::~Transport()
{
    // ...
}

void Transport::stop()
{
    playing.store(false);
}

void Transport::record()
{
    recording.store(true);
}
