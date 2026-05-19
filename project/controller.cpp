#include "controller.h"


namespace Project {

    Controller::Controller(Application& app, Model& m) : application(app), model(m)
    {

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

    void Controller::onAddTrack(TrackType type)
    {
        CtrlEvent event = type == TrackType::MIDI ? CtrlEvent::ADD_MIDI_TRACK : CtrlEvent::ADD_AUDIO_TRACK;
        int trackId = type == TrackType::MIDI ? model.addMidiTrack() : model.addAudioTrack();

        std::cout << "Controller => onAddTrack() " << std::endl;
        notify(event, trackId);
    }

    void Controller::onMuteTrack(int id, TrackType type, State state)
    {
        // logic here...
    }

    void Controller::onSoloTrack(int id, TrackType type, State state)
    {
        CtrlEvent event = state == State::ON ? CtrlEvent::SOLOED_TRACK : CtrlEvent::UNSOLOED_TRACK; 

        notify(event, id);
    }

    void Controller::onTrackSelected(int id)
    {
        notify(CtrlEvent::TRACK_SELECTED, id);
    }

    Controller::~Controller()
    {
        // ...
    }
}
