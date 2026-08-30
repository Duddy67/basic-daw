#ifndef TIME_CONVERTER_H
#define TIME_CONVERTER_H

#include <cstdint>

class Transport;
class TempoMap;

/*
 * Utility class
 */

class TimeConverter
{
    public:

        static int64_t getCurrentTick(const Transport& transport, const TempoMap& tempoMap, int sampleRate);
        static double getCurrentBeat(const Transport& transport, const TempoMap& tempoMap, int sampleRate);
        static void locateToTick(Transport& transport, const TempoMap& tempoMap, int64_t tick, int sampleRate);
};

#endif // TIME_CONVERTER_H
