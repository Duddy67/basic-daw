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
        trackIds.push_back(newTrackId);

        return trackIds.back();
    }

    void Model::deleteTrackId(int id)
    {

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

        // ...

        return newId;
    }
    
}
