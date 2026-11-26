#ifndef PROJECT_CONTROLLER_H
#define PROJECT_CONTROLLER_H

#include <vector>
#include <algorithm>
#include "../main.h"
#include "model.h"
#include "observer.h"
#include "view.h"
#include "../midi/scheduler.h"
#include "../core/engine.h"

class Application;
class Transport;
class TempoMap;

namespace Core {
    class Engine;
    class Track;
}

namespace Midi {
    class Scheduler;
}

namespace project {

    class Model;
    class Observer;
    class View;

    class Controller {

        Model& model;
        Transport& transport;
        TempoMap& tempoMap;
        Midi::Scheduler& scheduler;
        Core::Engine& engine;
        //Application& application;
        // List of all registered observers.
        std::vector<Observer*> observers;  
        bool recompile = false;

        void notify(CtrlEvent event, int index);

      public:

        //Controller(Model& m);
        Controller(Model& m, Transport& t, TempoMap& tm, Midi::Scheduler& s, Core::Engine& e);
        ~Controller();

        // Register an observer.
        void addObserver(Observer* observer) { observers.push_back(observer); }
        void removeObserver(Observer* observer);

        void onAddTrack(DataType type);
        void onToggleMute(int trackId, DataType type);
        void onToggleSolo(int trackId, DataType type);
        void onToggleArm(int trackId);
        void onTrackSelected(int id);
        void onPreviewNoteOn(int pitch, int velocity);
        void onPreviewNoteOff(int pitch);
        void notifyNotesChanged();
        void onPlay();
        void onStop();
        void locateToSample(uint64_t sample);
        size_t getTrackCount() const;
        bool needsRecompile() const { return recompile; }
        void doneRecompiling() { recompile = false; }
        Core::Track* getTrack(int id);
    };
}

#endif // PROJECT_CONTROLLER_H
