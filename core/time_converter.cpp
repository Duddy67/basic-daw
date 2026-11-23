#include "time_converter.h"
#include "../project/transport.h"
#include "../project/tempo_map.h"



int64_t TimeConverter::getCurrentTick(const Transport& transport, const TempoMap& tempoMap)
{
    uint64_t samples = transport.getPlayheadSample();
    return tempoMap.samplesToTicks(samples);
}

double TimeConverter::getCurrentBeat(const Transport& transport, const TempoMap& tempoMap)
{
    int64_t ticks = getCurrentTick(transport, tempoMap);
    return tempoMap.ticksToBeats(ticks);
}

/*void TimeConverter::locateToTick(Transport& transport, const TempoMap& tempoMap, int64_t tick)
{
    uint64_t samples = tempoMap.ticksToSamples(tick);
    transport.locateToSample(samples);
}*/

int TimeConverter::xOfBeat(double beat, const ViewState& viewState)
{
    return (int)std::round(beat * viewState.zoom) - viewState.horizontalOffset;
}

double TimeConverter::beatAtX(int x, const ViewState& viewState)
{
    return (x + viewState.horizontalOffset) / viewState.zoom; 
}

int TimeConverter::xOfTick(int64_t tick, const TempoMap& tempoMap, const ViewState& viewState)
{
    return xOfBeat(tempoMap.ticksToBeats(tick), viewState);
}

int64_t TimeConverter::tickAtX(int x, const TempoMap& tempoMap, const ViewState& viewState)
{
    return tempoMap.beatsToTick(beatAtX(x, viewState));
}
