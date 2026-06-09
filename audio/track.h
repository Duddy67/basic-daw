#ifndef AUDIO_TRACK_H
#define AUDIO_TRACK_H

#include "engine.h"
#include "../core/track.h"


namespace Audio {

    class Engine;

    class Track : public Core::Track {
        Engine& engine;

        public:

            Track(Engine& e, int id);
            ~Track() override;

            // Override virtual functions.
            TrackType getType() const override { return TrackType::AUDIO; }
    };
}

#endif // AUDIO_TRACK_H


