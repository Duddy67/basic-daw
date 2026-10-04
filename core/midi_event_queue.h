#ifndef MIDI_EVENT_QUEUE_H
#define MIDI_EVENT_QUEUE_H

#include <atomic>
#include "../constants.h"

/*
 * Basic queue that handles MIDI events generated through UI (piano roll etc...).
 */

class MidiEventQueue 
{
    static constexpr size_t CAP = 256;
    std::atomic<size_t> writeIdx{0};
    std::atomic<size_t> readIdx{0};
    QueuedMidiEvent buffer[CAP];

  public:

    // UI thread only.
    bool push(const QueuedMidiEvent& event)
    {
        size_t write = writeIdx.load(std::memory_order_relaxed);
        size_t read = readIdx.load(std::memory_order_acquire);

        if ((write + 1 ) % CAP == read) {
            // The queue is full. 
            return false;
        }

        buffer[write] = event;
        writeIdx.store((write + 1) % CAP, std::memory_order_release);

        return true;
    }

    // RT thread only.
    bool pop(QueuedMidiEvent& out)
    {
        size_t read = readIdx.load(std::memory_order_relaxed);
        size_t write = writeIdx.load(std::memory_order_acquire);

        if (read == write) {
            // The queue is empty. 
            return false;
        }

        out = buffer[read];
        readIdx.store((read + 1) % CAP, std::memory_order_release);

        return true;
    }
};

#endif // MIDI_EVENT_QUEUE_H
