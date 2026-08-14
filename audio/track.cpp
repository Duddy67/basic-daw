#include "track.h"


namespace Audio {

    Track::Track(int id) : Core::Track(id)
    {
    }

    Track::~Track()
    {
        // ...
    }

    void Track::mixInto(float* output, jack_nframes_t nframes)
    {

    }

    void Track::recordInto(const float* input, jack_nframes_t nframes)
    {

    }
}


