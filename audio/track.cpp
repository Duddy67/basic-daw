#include "track.h"


namespace Audio {

    Track::Track(Engine& e, int id) : engine(e)
    {
        this->id = id;
    }

    Track::~Track()
    {
        // ...
    }
}


