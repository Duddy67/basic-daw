#ifndef AUDIO_TRACK_H
#define AUDIO_TRACK_H

#include "processor.h"
#include "../core/track.h"


namespace Audio {

    class Track : public Core::Track {

        public:

            Track(int id);
            ~Track() override;

            // Override virtual functions.
            DataType getType() const override { return DataType::AUDIO; }
            void mixInto(float* output, jack_nframes_t nframes);
            void recordInto(const float* input, jack_nframes_t nframes);
    };
}

#endif // AUDIO_TRACK_H


