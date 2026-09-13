#include "processor.h"
#include "../audio/track.h"


namespace Audio {

    Processor::Processor(Application& app) : application(app)
    {
        // ...
    }

    Processor::~Processor()
    {
        // ...
    }

    void Processor::process(jack_nframes_t nframes, float* outLeft, float* outRight, const float* inLeft, const float* inRight)
    {
        if (!application.getTransport().isRolling()) {
            std::fill(outLeft, outLeft + nframes, 0.0f);
            std::fill(outRight, outRight + nframes, 0.0f);
            return;
        }

        // --- Playback ---
        static thread_local std::vector<float> interleavedOut;
        interleavedOut.resize(nframes * 2);
        std::fill(interleavedOut.begin(), interleavedOut.end(), 0.0f);

        auto& tracks = application.getProject()->getTracks();

        for (auto& track : tracks) {
            if (track->getType() == DataType::AUDIO) {
                auto* audioTrack = static_cast<Audio::Track*>(track.get());
                audioTrack->mixInto(interleavedOut.data(), nframes);
            }
        }

        for (jack_nframes_t i = 0; i < nframes; ++i) {
            outLeft[i]  = interleavedOut[i * 2];
            outRight[i] = interleavedOut[i * 2 + 1];
        }

        // --- Capture ---
        if (application.getTransport().isRecording()) {
            static thread_local std::vector<float> interleavedIn;
            interleavedIn.resize(nframes * 2);

            for (jack_nframes_t i = 0; i < nframes; ++i) {
                interleavedIn[i * 2]     = inLeft[i];
                interleavedIn[i * 2 + 1] = inRight[i];
            }

            for (auto& track : tracks) {
                if (track->getType() == DataType::AUDIO) {
                    auto* audioTrack = static_cast<Audio::Track*>(track.get());
                    audioTrack->recordInto(interleavedIn.data(), nframes);
                }
            }
        }
    }

    void Processor::createTrack()
    {
        // ...
    }

    void Processor::removeTrack()
    {
        // ...
    }
}

