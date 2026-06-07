#ifndef PROJECT_MODEL_H
#define PROJECT_MODEL_H

#include "../main.h"
#include "../audio/track.h"
#include "../midi/track.h"
#include <memory>

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
      std::vector<std::unique_ptr<Audio::Track>> audioTracks;  
      std::vector<std::unique_ptr<Midi::Track>> midiTracks;  
      int currentTrackId = -1;

      int getNewTrackId();
      void deleteTrackId(int id);

      public:

          Model(Application& app);
          ~Model();

          int addAudioTrack();
          int addMidiTrack();
          Audio::Track& getAudioTrack(int id);
          const std::vector<std::unique_ptr<Audio::Track>>& getAudioTracks() const { return audioTracks; }
          const std::vector<std::unique_ptr<Midi::Track>>& getMidiTracks() const { return midiTracks; }
          Midi::Track* getMidiTrack(int id);
          std::vector<int> getTrackIds() { return trackIds; }
          void midiToggleMute(int trackId);
          void audioToggleMute(int trackId);
          void midiToggleSolo(int trackId);
          size_t soloTrackCount() const { return soloTracks.size(); }

    };
}

#endif // PROJECT_MODEL_H
