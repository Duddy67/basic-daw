#include "model.h"


namespace Project {

    Model::Model(Application& app) : application(app)
    {
        // ...
    }

    Model::~Model()
    {
        // ...
    }

    int Model::getNewTrackId()
    {
        int newTrackId = (trackIds.empty()) ? 1 : trackIds.back() + 1;
        currentTrackId = newTrackId;
        trackIds.push_back(newTrackId);

        return trackIds.back();
    }

    void Model::deleteTrackId(int id)
    {

    }

    Midi::Track* Model::getMidiTrack(int id)
    {
        for (size_t i = 0; i < midiTracks.size(); i++) {
            if (midiTracks[i]->getId() == id) {
                // Return raw pointer.
                return midiTracks[i].get();
            }
        }

        // In case the track doesn't exist.
        return nullptr;
    }

    int Model::addAudioTrack()
    {
        int newId = getNewTrackId();
        auto track = std::make_unique<Audio::Track>(application.getAudioEngine(), newId);
        audioTracks.push_back(std::move(track));
        // ....
        //std::cout << "Model => addTrack(): " << track->getId() << std::endl;
        return newId;

        
    }

    int Model::addMidiTrack()
    {
        int newId = getNewTrackId();
        auto track = std::make_unique<Midi::Track>(application.getMidiEngine(), newId);
        midiTracks.push_back(std::move(track));

        // ...

        return newId;
    }
    
    void Model::midiToggleMute(int trackId)
    {
        for (size_t i = 0; i < midiTracks.size(); i++) {
            if (midiTracks[i]->getId() == trackId) {
                midiTracks[i]->toggleMute();
            }
        }
    }
    
    void Model::audioToggleMute(int trackId)
    {
        // ...
    }

    void Model::midiToggleSolo(int trackId)
    {
        for (size_t i = 0; i < midiTracks.size(); i++) {
            if (midiTracks[i]->getId() == trackId) {
                midiTracks[i]->toggleSolo();

                if (midiTracks[i]->isSoloed()) {
                    soloTracks.push_back(midiTracks[i]->getId());
                }
                else {
                    soloTracks.erase(soloTracks.begin() + i);
                }
            }
        }
    }
}
