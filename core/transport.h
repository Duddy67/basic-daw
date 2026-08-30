#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <atomic>
#include "../main.h"

class Application;

class Transport {
    Application& application;
    std::atomic<uint64_t> playheadSample{0};
    std::atomic<bool> playing{false};
    std::atomic<bool> recording{false};

    public:
        Transport(Application& app);
        ~Transport();

        void play();
        void stop();
        void record();
        bool isPlaying() const { return playing.load(); }
        bool isRecording() const { return recording.load(); }
        // Returns the current playhead position in samples.
        uint64_t getPlayheadSample() const { return playheadSample.load(); }
        // Advances the playhead by the number of frames just processed.
        // This is called every audio callback.
        void advancePlayhead(uint64_t frameCount) { playheadSample.fetch_add(frameCount, std::memory_order_relaxed); }
        // Jumps the playhead to an absolute sample position.
        // Called when the user clicks the timeline or relocates the song.
        void locateToSample(uint64_t sample) { playheadSample.store(sample, std::memory_order_release); }
};

#endif // TRANSPORT_H
