#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>
#include <string>

constexpr unsigned int MODAL_WND_POS = 20;
constexpr unsigned int BUTTON_WIDTH = 80;
constexpr unsigned int BUTTON_HEIGHT = 40;
constexpr unsigned int MICRO_SPACE = 5;
constexpr unsigned int BORDER_INTERSTICE = 2;
constexpr unsigned int TINY_SPACE = 10;
constexpr unsigned int SMALL_SPACE = 40;
constexpr unsigned int MEDIUM_SPACE = 80;
constexpr unsigned int LARGE_SPACE = 160;
constexpr unsigned int XLARGE_SPACE = 320;
constexpr unsigned int TEXT_SIZE = 13;
constexpr unsigned int SCROLLBAR_HEIGHT = 15;
constexpr unsigned int SCROLLBAR_MARGIN = 10;
constexpr unsigned int TAB_BORDER_THICKNESS = 10;

// --- Custom types ---

enum class Direction { LEFT, RIGHT, UP, DOWN, NONE };

enum class TimeFormat { HH_MM_SS_SSS, MM_SS_SSS, SS_SSS };

enum class EditID {
    COPY, PASTE, CUT, DELETE, 
    UNDO, REDO, NONE
};

enum class FileID {
    NEW, OPEN, CLOSE, SAVE, 
    SAVE_AS, QUIT
};

enum class TrackID {
    ADD, REMOVE
};

enum class TrackType {
    MIDI, AUDIO
};

enum class TransportID { PLAY, STOP, PAUSE, RECORD, LOOP };

enum class Action {ACTIVATE, DEACTIVATE};

enum class State {
    ON, OFF
};

enum class MenuItemID {
    FILE_SUB, FILE_NEW, FILE_OPEN, FILE_SAVE, FILE_SAVE_AS, FILE_QUIT, EDIT_SUB,
    EDIT_UNDO, EDIT_REDO, EDIT_DELETE, EDIT_COPY, EDIT_PASTE, EDIT_CUT, TRACK_SUB,
    TRACK_ADD, TRACK_REMOVE
};

struct Selection {
    int start, end;
};

inline std::map<EditID, std::string> EditLabels {
    {EditID::CUT, "Cut"},
    {EditID::PASTE, "Paste"},
    {EditID::NONE, ""}
};

enum class CtrlEvent {
    ADD_MIDI_TRACK, ADD_AUDIO_TRACK, REMOVE_TRACK, MUTED_TRACK,
    UNMUTED_TRACK, SOLOED_TRACK, UNSOLOED_TRACK, TRACK_SELECTED
};

inline std::map<MenuItemID, std::string> MenuLabels {
    {MenuItemID::FILE_SUB, "File"},
    {MenuItemID::FILE_NEW, "File/&New"},
    {MenuItemID::FILE_OPEN, "File/&Open"},
    {MenuItemID::FILE_SAVE, "File/&Save"},
    {MenuItemID::FILE_SAVE_AS, "File/_&Save as"},
    {MenuItemID::FILE_QUIT, "File/&Quit"},
    {MenuItemID::EDIT_SUB, "Edit"},
    {MenuItemID::EDIT_UNDO, "Edit/&Undo"},
    {MenuItemID::EDIT_REDO, "Edit/&Redo"},
    {MenuItemID::EDIT_DELETE, "Edit/Delete"},
    {MenuItemID::EDIT_COPY, "Edit/&Copy"},
    {MenuItemID::EDIT_PASTE, "Edit/&Paste"},
    {MenuItemID::EDIT_CUT, "Edit/&Cut"},
    {MenuItemID::TRACK_SUB, "Track"},
    {MenuItemID::TRACK_ADD, "Track/&Add"},
    {MenuItemID::TRACK_REMOVE, "Track/&Remove"}
};

#endif
