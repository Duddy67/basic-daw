#include "controller.h"


namespace project {

    Controller::Controller(Model& m, Transport& t, TempoMap& tm, Midi::Scheduler& s, Core::Engine& e) : 
        model(m), transport(t), tempoMap(tm), scheduler(s), engine(e)
    {
        // ...
    }

    Controller::~Controller()
    {
        // ...
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

    /*
     * Jumps the playhead to an absolute sample position.
     * Called when the user clicks the timeline or relocates the song.
     */
    void Controller::locateToSample(uint64_t sample)
    {
        // Update the playhead position.
        //application.getTransport().setPlayheadSample(sample);
        transport.setPlayheadSample(sample);
        // Update the playhead position in MIDI tracks.
        //application.getMidiScheduler().relocate(sample);
        scheduler.relocate(sample);

        notify(CtrlEvent::PLAYHEAD_MOVED, 0);
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

    void Controller::onPlay()
    {
        //application.getTransportBar().onPlay();
        transport.play();
    }

    void Controller::onStop()
    {
        //application.getTransportBar().onStop();
        transport.stop();
    }

    /*
     * Stores the note on messages coming from UI (piano roll...) into the UI queue.
     */
    void Controller::onPreviewNoteOn(int pitch, int velocity)
    {
        QueuedMidiEvent event;
        // Build the event from the given pitch and velocity.
        event.size = 3;
        event.bytes[0] = 0x90 | (0 & 0x0F);
        event.bytes[1] = (uint8_t)pitch;
        event.bytes[2] = (uint8_t)velocity;

        engine.pushUiMidiEvent(event);
    }

    /*
     * Stores the note off messages coming from UI (piano roll...) into the UI queue.
     */
    void Controller::onPreviewNoteOff(int pitch)
    {
        QueuedMidiEvent event;
        // Build the event from the given pitch.
        event.size = 3;
        event.bytes[0] = 0x80 | (0 & 0x0F);
        event.bytes[1] = (uint8_t)pitch;
        // Release velocity.
        event.bytes[2] = 0x40;

        engine.pushUiMidiEvent(event);
    }

    void Controller::notifyNotesChanged() {
        notify(CtrlEvent::MIDI_NOTES_CHANGED, 0);

        if (!getTransport().isRolling()) {
            // Compile the edited MIDI notes.
            //application.getMidiScheduler().compile(*this);
            scheduler.compile(*this);
        }
        else {
            recompile = true;
        }
    }

    // Getters

    Transport& Controller::getTransport()
    {
        //return application.getTransport();
        return transport;
    }

    TempoMap& Controller::getTempoMap()
    {
        //return application.getTempoMap();
        return tempoMap;
    }

    Core::Engine& Controller::getEngine()
    {
        //return application.getCoreEngine();
        return engine;
    }

    View& Controller::getView()
    {
        //return application.getProjectView();
        return model.getApplication().getProjectView();
    }

    bool Controller::hasView()
    {
        //return application.hasView();
        return model.getApplication().hasView();
    }

    ViewState& Controller::getViewState()
    {
        return model.getViewState();
    }

    bool Controller::isSnapToGrid()
    {
        //return application.getGridBar().isSnapToGrid();
        return model.getApplication().getGridBar().isSnapToGrid();
    }

    size_t Controller::getTrackCount() const
    {
        // Use the model's method.
        return model.trackCount();
    }

    Core::Track* Controller::getTrack(int id)
    {
        // Use the model's method.
        return model.getTrack(id);
    }
}
