#ifndef TIME_CONVERTER_H
#define TIME_CONVERTER_H

#include <cstdint>
#include "../constants.h"

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
        static int xOfBeat(double beat, const ViewState& viewState);
        static double beatAtX(int x, const ViewState& viewState);
        static int xOfTick(int64_t tick, const TempoMap& tempoMap, const ViewState& viewState);
        static int64_t tickAtX(int x, const TempoMap& tempoMap, const ViewState& viewState);
};

#endif // TIME_CONVERTER_H
