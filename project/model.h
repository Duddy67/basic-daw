#ifndef PROJECT_MODEL_H
#define PROJECT_MODEL_H

#include <memory>
#include "../main.h"
#include "../core/track.h"

class Application;

namespace Audio {
    class Track;
}

namespace Midi {
    class Track;
}

namespace Project {

    class Model {

      Application& application;
      std::vector<int> trackIds;  
      std::vector<int> soloTracks;  
      std::vector<std::unique_ptr<Core::Track>> tracks;  
      int currentTrackId = -1;

      int getNewTrackId();
      void deleteTrackId(int id);

      public:

          Model(Application& app);
          ~Model();

          int addAudioTrack();
          int addMidiTrack();
          const std::vector<std::unique_ptr<Core::Track>>& getTracks() const { return tracks; }
          Core::Track* getTrack(int id); 
          std::vector<int> getTrackIds() { return trackIds; }
          void toggleMute(int trackId);
          void toggleSolo(int trackId);
          void toggleArm(int trackId);
          size_t trackCount() const { return tracks.size(); }
          size_t soloTrackCount() const { return soloTracks.size(); }
          Application& getApplication() const { return application; }
    };
}

#endif // PROJECT_MODEL_H
