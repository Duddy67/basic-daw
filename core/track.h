#ifndef CORE_TRACK_H
#define CORE_TRACK_H

#include <memory>
#include <vector>
#include "../constants.h"


namespace Core {

    class Track {

        protected:

            int id = 0;
            bool mute = false;
            bool solo = false;
            bool arm = false;
            
        public:

            Track(int id) : id(id) {}
            virtual ~Track() = default;

            // Common operations - non-virtual (can be overridden if needed)
            int getId() const { return id; }
            void toggleMute() { mute = !mute; }
            bool isMuted() const { return mute; }
            void toggleSolo() { solo = !solo; }
            bool isSoloed() const { return solo; }
            void toggleArm() { arm = !arm; }
            bool isArmed() const { return arm; }

            // Pure virtual. Must be implemented by derived classes.
            virtual TrackType getType() const = 0;
    };
}

#endif // CORE_TRACK_H
