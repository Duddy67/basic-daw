#include "model.h"
#include "../audio/track.h"
#include "../midi/track.h"


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

    /*
     * Removes the given id from the track id list.
     */
    void Model::deleteTrackId(int id)
    {
        trackIds.erase(std::remove(trackIds.begin(), trackIds.end(), id), trackIds.end());
    }

    int Model::addAudioTrack()
    {
        int newId = getNewTrackId();
        auto track = std::make_unique<Audio::Track>(newId);
        tracks.push_back(std::move(track));
        // ....
        return newId;
    }

    int Model::addMidiTrack()
    {
        int newId = getNewTrackId();
        auto track = std::make_unique<Midi::Track>(newId);
        tracks.push_back(std::move(track));

        // ...

        return newId;
    }
    
    Core::Track* Model::getTrack(int id)
    {
        for (size_t i = 0; i < tracks.size(); i++) {
            if (tracks[i]->getId() == id) {
                // Return raw pointer.
                return tracks[i].get();
            }
        }

        // In case the track doesn't exist.
        return nullptr;
    }

    void Model::toggleMute(int trackId)
    {
        if (auto track = getTrack(trackId)) {
            track->toggleMute();
        std::cout << "Model => toggleMute() " << std::endl;
        }
    }

    void Model::toggleSolo(int trackId)
    {
        if (auto track = getTrack(trackId)) {
            track->toggleSolo();

            // Check the button state (activated/deactivated) and set 
            // the solo list accordingly.

            if (track->isSoloed()) {
                // Add the given track id to the solo list.
                soloTracks.push_back(track->getId());
            }
            else {
                // Remove the given track id from the solo list.
                soloTracks.erase(std::remove(soloTracks.begin(), soloTracks.end(), trackId), soloTracks.end());
            }
        }
    }

    void Model::toggleArm(int trackId)
    {
        if (auto track = getTrack(trackId)) {
            track->toggleArm();
        }
    }
}
