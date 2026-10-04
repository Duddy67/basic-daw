#ifndef PITCH_CONVERTER_H
#define PITCH_CONVERTER_H

#include <cstdint>
#include "../constants.h"

/*
 * Utility class
 */

class PitchConverter
{
    public:

        static int yOfPitch(int pitch, int canvasY, int canvasH, const ViewState& viewState)
        {
            int lowestPitch = viewState.pitchVerticalOffset / viewState.keyHeight;
            int rowFromBottom = pitch - lowestPitch;

            return (canvasY + canvasH) - (rowFromBottom + 1) * viewState.keyHeight;
        }

        static int pitchAtY(int y, int canvasY, int canvasH, const ViewState& viewState)
        {
            // Scroll in rows.
            int lowestPitch = viewState.pitchVerticalOffset / viewState.keyHeight;
            int rowFromBottom = (canvasY + canvasH - y - 1) / viewState.keyHeight;

            return lowestPitch + rowFromBottom;
        }

        static bool isBlackKey(int pitch)
        {
            int n = ((pitch % 12) + 12) % 12;

            return n == 1 || n == 3 || n == 6 || n == 8 || n == 10;
        }
};

#endif // PITCH_CONVERTER_H
