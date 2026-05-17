#ifndef PROJECT_MODEL_H
#define PROJECT_MODEL_H

#include "../main.h"
#include "../audio/track.h"
#include <memory>

class Application;

namespace Audio {
    class Track;
}

namespace Project {

    class Model {

      Application& application;
      std::vector<int> trackIds;  
      std::vector<std::unique_ptr<Audio::Track>> audioTracks;  

      int getNewTrackId();
      void deleteTrackId(int id);

      public:

          Model(Application& app);
          ~Model();

          int addAudioTrack();
          int addMidiTrack();
          Audio::Track& getAudioTrack(int id);
          //const std:vector<Audio::Track>& getAudioTracks() const;

    };
}

#endif // PROJECT_MODEL_H
