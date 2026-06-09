#include "track.h"


namespace Audio {

    Track::Track(Engine& e, int id) : Core::Track(id), engine(e)
    {
    }

    Track::~Track()
    {
        // ...
    }
}


