#include "time_converter.h"
#include "transport.h"
#include "tempo_map.h"



int64_t TimeConverter::getCurrentTick(const Transport& transport, const TempoMap& tempoMap, int sampleRate)
{
    uint64_t samples = transport.getPlayheadSample();
    return tempoMap.samplesToTicks(samples, sampleRate);
}

double TimeConverter::getCurrentBeat(const Transport& transport, const TempoMap& tempoMap, int sampleRate)
{
    int64_t ticks = getCurrentTick(transport, tempoMap, sampleRate);
    return tempoMap.ticksToBeats(ticks);
}

void TimeConverter::locateToTick(Transport& transport, const TempoMap& tempoMap, int64_t tick, int sampleRate)
{
    uint64_t samples = tempoMap.ticksToSamples(tick, sampleRate);
    transport.locateToSample(samples);
}
