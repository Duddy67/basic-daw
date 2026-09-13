#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>
#include <string>
#include <vector>
#include <cstdint>

constexpr unsigned int MODAL_WND_POS = 20;
constexpr unsigned int MENU_BAR_HEIGHT = 40;
constexpr unsigned int TOOL_BAR_HEIGHT = 60;
constexpr unsigned int TRANSPORT_BAR_HEIGHT = 50;
constexpr unsigned int RULER_HEIGHT = 40;
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
constexpr unsigned int SCROLLBAR_HEIGHT = 16;
constexpr unsigned int SCROLLBAR_MARGIN = 10;
constexpr unsigned int TAB_BORDER_THICKNESS = 10;
constexpr unsigned int MAX_MIDI_CHANNELS = 16;
constexpr unsigned int SAMPLE_RATE = 44100;
//constexpr unsigned int PPQ = 960;
constexpr double SECONDS_PER_MINUTE = 60.0;
constexpr double DEFAULT_BPM = 120.0;
constexpr int DEFAULT_NUMERATOR = 4;
constexpr int DEFAULT_DENOMINATOR = 4;
constexpr int DEFAULT_PPQ = 480; // in ticks
constexpr double MIN_ZOOM = 5.0; // 5 pixels per beat (zoomed way out)
constexpr double MAX_ZOOM = 2000.0; // 2000 pixels per beat (zoomed way in)
constexpr const char* CONFIG_FILENAME = "config.json";

// --- Custom types ---

enum class Direction {
    LEFT, RIGHT, UP, DOWN, NONE
};

enum class TimeFormat {
    HH_MM_SS_SSS, MM_SS_SSS, SS_SSS
};

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

enum class DataType {
    MIDI, AUDIO
};

enum class TransportID { PLAY, STOP, PAUSE, RECORD, LOOP };

enum class Action {ACTIVATE, DEACTIVATE};

enum class State {
    ON, OFF
};

enum class ConnectionType {
    INPUT, INPUT_EXCLUDE_CLIENT, OUTPUT, OUTPUT_EXCLUDE_CLIENT
};

enum class PortType {
    INPUT, INPUT_EXCLUDE_CLIENT, OUTPUT, OUTPUT_EXCLUDE_CLIENT
};

enum class MenuItemID {
    FILE_SUB, FILE_NEW, FILE_OPEN, FILE_SAVE, FILE_SAVE_AS, FILE_QUIT, EDIT_SUB,
    EDIT_UNDO, EDIT_REDO, EDIT_DELETE, EDIT_COPY, EDIT_PASTE, EDIT_CUT, TRACK_SUB,
    TRACK_ADD, TRACK_REMOVE, SETTINGS_SUB, SETTINGS_AUDIO, SETTINGS_MIDI
};

enum class CtrlEvent {
    ADD_MIDI_TRACK, ADD_AUDIO_TRACK, REMOVE_TRACK, MUTED_TRACK,
    UNMUTED_TRACK, SOLOED_TRACK, UNSOLOED_TRACK, TRACK_SELECTED
};

struct Selection {
    int start, end;
};

struct MidiEvent {
    uint64_t samplePosition;
    std::vector<unsigned char> message;
};

struct TempoChange {
    // Beat position where this tempo takes effect.
    double beat; 
    double bpm;
};

struct TimeSignatureChange {
    // Beat position where this signature takes effect.
    double beat;     
    // Beats per bar (eg: 4 for 4/4).
    int numerator;   
    // Note value per beat (usually 4, meaning quarter note).
    int denominator; 
};

struct ViewState {
    // Pixel per beat (eg: 100.0 = 1 beat = 100 pixels).
    double zoom = 100.0;
    // How many pixels the view is scrolled to the right.
    int horizontalOffset = 0;
    // How many pixels the view is scrolled down.
    int verticalOffset = 0;
};

inline std::map<EditID, std::string> EditLabels {
    {EditID::CUT, "Cut"},
    {EditID::PASTE, "Paste"},
    {EditID::NONE, ""}
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
    {MenuItemID::TRACK_REMOVE, "Track/&Remove"},
    {MenuItemID::SETTINGS_SUB, "Settings"},
    {MenuItemID::SETTINGS_AUDIO, "Settings/&Audio"},
    {MenuItemID::SETTINGS_MIDI, "Settings/&MIDI"}
};

#endif
