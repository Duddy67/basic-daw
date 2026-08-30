#include "controller.h"


namespace Project {

    Controller::Controller(Application& app, Model& m) : application(app), model(m)
    {

    }

    Controller::~Controller()
    {
        // ...
    }

    Transport& Controller::getTransport()
    {
        return application.getTransport();
    }

    TempoMap& Controller::getTempoMap()
    {
        return application.getTempoMap();
    }

    Core::Engine& Controller::getEngine()
    {
        return application.getCoreEngine();
    }

    ViewState& Controller::getViewState()
    {
        return application.getViewState();
    }

    /*
     * Unregister an observer (important to prevent crashes when views are deleted).
     */
    void Controller::removeObserver(Observer* observer)
    {
        auto it = std::find(observers.begin(), observers.end(), observer);

        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    void Controller::notify(CtrlEvent event, int index)
    {
        for (auto* observer : observers) {
            observer->onCtrlEvent(event, index);  
        }
    }

    void Controller::onAddTrack(DataType type)
    {
        CtrlEvent event = type == DataType::MIDI ? CtrlEvent::ADD_MIDI_TRACK : CtrlEvent::ADD_AUDIO_TRACK;
        int trackId = type == DataType::MIDI ? model.addMidiTrack() : model.addAudioTrack();

        std::cout << "Controller => onAddTrack() " << std::endl;

        notify(event, trackId);
    }

    void Controller::onToggleMute(int trackId, DataType type)
    {
        model.toggleMute(trackId);
    }

    void Controller::onToggleSolo(int trackId, DataType type)
    {
        model.toggleSolo(trackId);
        //notify(event, trackId);
    }

    void Controller::onToggleArm(int trackId)
    {
        model.toggleArm(trackId);
        //notify(event, trackId);
    }

    void Controller::onTrackSelected(int id)
    {
        std::cout << "onTrackSelected(): " << id << std::endl;
        notify(CtrlEvent::TRACK_SELECTED, id);
    }
}
