#include "track.h"


namespace Midi {

    Track::Track(Engine& e, int id) : Core::Track(id), engine(e)
    {
        // ...
    }

    Track::~Track()
    {
        // ...
    }

    void Track::processMessage(const std::vector<unsigned char>& message, double deltaTime) const
    {
        // First, check for solo tracks.
        if (engine.getProject()->soloTrackCount() && !isSoloed()) {
            // This track is not part of the currently soloed tracks.
            // Don't play.
            return;
        }

        if (!isMuted()) {
            unsigned int nBytes = message.size();
            for (unsigned int i = 0; i < nBytes; i++) {
                std::cout << "Byte " << i << " = " << (int)message.at(i) << ", ";
            }

            if (nBytes > 0) {
                std::cout << "stamp = " << deltaTime << ", ";
            }

            std::cout << "track id = " << getId() << std::endl;
        }
    }
}



