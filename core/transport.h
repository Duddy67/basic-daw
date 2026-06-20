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
        uint64_t getPlayheadSample() const { return playheadSample.load(); }
        void advancePlayhead(uint64_t frameCount) { playheadSample.fetch_add(frameCount, std::memory_order_relaxed); }

};

#endif // TRANSPORT_H
