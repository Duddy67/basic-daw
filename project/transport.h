#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <atomic>
#include <functional>
//#include "../main.h"


class Transport {
    std::atomic<uint64_t> playheadSample{0};
    std::atomic<bool> playing{false};
    std::atomic<bool> recording{false};

    std::function<void()> onPlayStarted;
    std::function<void()> onPlayStopped;

    public:
        Transport();
        ~Transport();

        void setPlayCallbacks(std::function<void()> start, std::function<void()> stop); 
        void play();
        void stop();
        void record();
        bool isPlaying() const { return playing.load(); }
        bool isRecording() const { return recording.load(); }
        bool isRolling() const { return playing.load() || recording.load(); }
        // Returns the current playhead position in samples.
        uint64_t getPlayheadSample() const { return playheadSample.load(); }
        // Updates the current playhead position in samples.
        void setPlayheadSample(uint64_t sample) { playheadSample.store(sample, std::memory_order_release); }
        // Advances the playhead by the number of frames just processed.
        // This is called every audio callback.
        void advancePlayhead(uint64_t frameCount) { playheadSample.fetch_add(frameCount, std::memory_order_relaxed); }
};

#endif // TRANSPORT_H
