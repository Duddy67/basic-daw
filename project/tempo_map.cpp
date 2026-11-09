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

// ------------------- Tick <=> Time conversions  -------------------

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

// ------------------- Tick <=> Sample conversions  -------------------

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

// ------------------- Beat <=> Sample conversions  -------------------

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

// ------------------- Beat <=> Time conversions  -------------------

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

/*
 * Helper - Find the index of the time signature that applies at the given beat.
 */
int TempoMap::getTimeSignatureIndex(double beat) const
{
    if (timeSignatures.empty()) {
        return -1;
    }

    int id = 0;

    for (size_t i = 0; i < timeSignatures.size(); ++i) {
        if (timeSignatures[i].beat <= beat) {
            id = i;
        }
        else {
            break;
        }
    }

    return id;
}

// ------------------- Bar calculations -------------------

/*
 * Returns the bar number (1-indexed) and the beat position within that bar.
 * (0.0 = downbeat, 1.0 = second beat, etc.)
 */
std::pair<int, double> TempoMap::getBarBeat(double beat) const
{
    double barStart = getBarStart(beat);
    int barNum = getBarNumber(beat);
    // Fractional beats from the bar start.
    double beatsInBar = beat - barStart;

    return {barNum, beatsInBar};
}

double TempoMap::getBarStart(double beat) const
{
    if (timeSignatures.empty()) {
        return 0.0;
    }

    if (beat < 0.0) {
        return 0.0;
    }

    int id = getTimeSignatureIndex(beat);
    const TimeSignatureChange& signature = timeSignatures[id];
    double beatsPerBar = (double)signature.numerator;

    // How many beats from the signature start to the given beat ?
    double offsetFromSignatureStart = beat - signature.beat;

    // Round down to the nearest full bar.
    double barsFromSignatureStart = std::floor(offsetFromSignatureStart / beatsPerBar);

    return signature.beat + (barsFromSignatureStart * beatsPerBar);
}

std::vector<double> TempoMap::getBarLines(double startBeat, double endBeat) const
{
    std::vector<double> lines;

    if (startBeat > endBeat) {
        return lines;
    }

    // Find the first bar start that is >= startBeat.
    double bar = getBarStart(startBeat);

    if (bar < startBeat) {
        // Move to the next bar.
        int id = getTimeSignatureIndex(bar);

        if (id < 0) {
            return lines;
        }

        double beatsPerBar = (double)timeSignatures[id].numerator;
        bar += beatsPerBar;
    }

    while (bar <= endBeat) {
        lines.push_back(bar);
        int id = getTimeSignatureIndex(bar);

        if (id < 0) {
            break;
        }

        double beatsPerBar = (double)timeSignatures[id].numerator;
        bar += beatsPerBar;
    }

    return lines;
}

int TempoMap::getBarNumber(double beat) const
{
    if (timeSignatures.empty()) {
        return 1;
    }

    if (beat < 0.0) {
        return 1;
    }

    int barCount = 1;
    double currentBeat = 0.0;

    for (size_t i = 0; i < timeSignatures.size(); ++i) {
        const TimeSignatureChange& signature = timeSignatures[i];
        double beatsPerBar = (double)signature.numerator;

        // If this is not the last signature, see if the next one is reached. 
        if (i + 1 < timeSignatures.size()) {
            const TimeSignatureChange& nextSignature = timeSignatures[i + 1];
            double segmentBeats = nextSignature.beat - currentBeat;
            int barsInSegment = (int)std::floor(segmentBeats / beatsPerBar);
            barCount += barsInSegment;

            if (beat < nextSignature.beat) {
                double offset = beat - currentBeat;
                int barsToAdd = (int)std::floor(offset / beatsPerBar);

                // The whole segment is already added, so subtract the exess.
                return barCount - barsInSegment + barsToAdd;
            }
        }
        else {
            // Last signature section (goes to infinity).
            double offset = beat - currentBeat;
            int barsToAdd = (int)std::floor(offset / beatsPerBar);

            return barCount + barsToAdd;
        }

        currentBeat = signature.beat;
    }

    // Fallback.
    return barCount;
}

