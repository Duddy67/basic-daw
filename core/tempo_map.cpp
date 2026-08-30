#include "tempo_map.h"


TempoMap::TempoMap(Application& app) : application(app)
{
    // Default: 120 BPM, 4/4 time starting at beat 0.
    tempos.push_back({0.0, DEFAULT_BPM});
    timeSignatures.push_back({0.0, DEFAULT_NUMERATOR, DEFAULT_DENOMINATOR});
}

TempoMap::~TempoMap()
{
    // ...
}

/*
 * Direct conversion from absolute MIDI ticks to seconds.
 */
double TempoMap::ticksToSeconds(int64_t ticks) const
{
    return beatToSeconds(ticksToBeats(ticks));
}

/*
 * Direct conversion from seconds to absolute MIDI ticks.
 */
uint64_t TempoMap::secondsToTicks(double seconds) const
{
    return beatsToTick(secondsToBeats(seconds));
}

/*
 * Direct conversion from absolute MIDI ticks to audio samples.
 */
int64_t TempoMap::ticksToSamples(int64_t ticks, int sampleRate) const
{
    double seconds = ticksToSeconds(ticks);
    return (int64_t)(seconds * sampleRate);
}

/*
 * Direct conversion from audio samples to absolute MIDI ticks.
 */
int64_t TempoMap::samplesToTicks(int64_t samples, int sampleRate) const
{
    double seconds = (double)samples / (double)sampleRate;
    return secondsToTicks(seconds);
}

double TempoMap::samplesToBeats(jack_nframes_t samples, int sampleRate)
{
    // Convert samples to seconds.
    double seconds = (double)samples / (double) sampleRate;
    // Convert seconds to beats.
    return secondsToBeats(seconds);
}

double TempoMap::beatToSamples(double beat, int sampleRate)
{
    // Convert the given beat to seconds.
    double seconds = beatToSeconds(beat);
    // Convert seconds to samples.
    return (jack_nframes_t)(seconds * sampleRate);
}

double TempoMap::secondsToBeats(double seconds) const
{
    // Should never happen.
    if (tempos.empty()) {
        return 0.0;
    }

    if (seconds < 0.0) {
        return 0.0;
    }

    double remaining = seconds;
    double currentBeat = 0.0;
    double currentBpm = tempos[0].bpm;

    // Iterate over tempo segments, except the last one.
    for (size_t i = 0; i < tempos.size() - 1; ++i) {
        const TempoChange& next = tempos[i + 1];
        double segmentBeats = next.beat - currentBeat;
        double segmentDuration = segmentBeats * (SECONDS_PER_MINUTE / currentBpm);

        if (remaining <= segmentDuration) {
            double fractionalBeats = remaining / (SECONDS_PER_MINUTE / currentBpm);
            return currentBeat + fractionalBeats;
        }

        // Subtract the whole segment and move to the next tempo.
        remaining -= segmentDuration;
        currentBeat = next.beat;
        currentBpm = next.bpm;
    }

    // If we reach here, we're beyond all tempo changes.
    // Use the last tempo to convert the remaining seconds to beats.
    double beatsToAdd = remaining / (SECONDS_PER_MINUTE / currentBpm);

    return currentBeat + beatsToAdd;
}

double TempoMap::beatToSeconds(double beat) const
{
    // Should never happen.
    if (tempos.empty()) {
        return 0.0;
    }

    double totalSeconds = 0.0;
    double currentBeat = 0.0;
    double currentBpm = tempos[0].bpm;

    // Iterate through tempo changes
    for (size_t i = 0; i < tempos.size(); ++i) {
        const TempoChange& tempoChange = tempos[i];

        // If the given beat is before this tempo change, break.
        if (beat <= tempoChange.beat) {
            // Remaining duration from the current beat to the given beat.
            double beatsToTravel = beat - currentBeat;

            if (beatsToTravel > 0) {
                totalSeconds += beatsToTravel * (SECONDS_PER_MINUTE / currentBpm);
            }

            return totalSeconds;
        }

        // Add full segment from currentBeat to this tempo change.
        double segmentBeats = tempoChange.beat - currentBeat;

        if (segmentBeats > 0) {
            totalSeconds += segmentBeats * (SECONDS_PER_MINUTE / currentBpm);
        }

        // Move to the new tempo.
        currentBeat = tempoChange.beat;
        currentBpm = tempoChange.bpm;
    }

    // If we get here, the given beat is after the last tempo change.
    double beatsToTravel = beat - currentBeat;

    if (beatsToTravel > 0) {
        totalSeconds += beatsToTravel * (SECONDS_PER_MINUTE / currentBpm);
    }

    return totalSeconds;
}
