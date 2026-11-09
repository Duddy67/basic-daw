#ifndef TEMPO_MAP_H
#define TEMPO_MAP_H

#include <jack/jack.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "../constants.h"

class Application;

class TempoMap 
{
        Application& application;
        std::vector<TempoChange> tempos;
        // Manages time signature vector as well because it shares the same beat-coordinate system.
        std::vector<TimeSignatureChange> timeSignatures;
        int ppq = DEFAULT_PPQ;

        // Helper - Find the index of the time signature that applies at the given beat.
        int getTimeSignatureIndex(double beat) const;

    public:

        TempoMap(Application& app);
        ~TempoMap();

        void setPPQ(int newPpq) { ppq = newPpq; }
        int getPPQ() const { return ppq; }
        // Converts ticks to musical beats.
        double ticksToBeats(int64_t ticks) const { return (double)ticks / (double)ppq; }
        // Converts musical beats to the nearest tick.
        // note: llround avoids floating-point errors.
        int64_t beatsToTick(double beats) const { return (int64_t)std::llround(beats * ppq); } 
        double ticksToSeconds(int64_t ticks) const;
        uint64_t secondsToTicks(double seconds) const;
        int64_t ticksToSamples(int64_t ticks, int sampleRate) const;
        int64_t samplesToTicks(int64_t samples, int sampleRate) const;
        double samplesToBeats(jack_nframes_t samples, int sampleRate);
        double beatToSamples(double beat, int sampleRate);
        double secondsToBeats(double seconds) const;
        double beatToSeconds(double beat) const;
        std::pair<int, double> getBarBeat(double beat) const;
        double getBarStart(double beat) const;
        int getBarNumber(double beat) const;
        std::vector<double> getBarLines(double startBeat, double endBeat) const;
};

#endif // TEMPO_MAP_H
