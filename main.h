#ifndef MAIN_H
#define MAIN_H
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <errno.h>
#include <cstdlib>
#include <iostream>
#include "constants.h"
#include "config/config.h"
#include "dialogs/new.h"
#include "dialogs/audio_settings.h"
#include "dialogs/midi_settings.h"
#include "dialogs/add_track.h"
#include "project/model.h"
#include "project/controller.h"
#include "project/view.h"
#include "audio/engine.h"
#include "midi/engine.h"
#include "core/transport.h"
#include "core/midi_scheduler.h"
#include "widgets/transport_bar.h"
#include "json.hpp"

using json = nlohmann::json;

// Forward class declarations.

namespace Project {
    class Controller;
    class View;
}

namespace Audio {
    class Engine;
}

namespace Midi {
    class Engine;
}

class Transport;
class MidiScheduler;


class Application : public Fl_Double_Window 
{
    Fl_Menu_Bar* menu = nullptr;
    Fl_Group* toolbar = nullptr;
    Fl_Menu_Item* menuItem = nullptr;
    Fl_Menu_Item* undoMenuItem = nullptr;
    Fl_Menu_Item* redoMenuItem = nullptr;
    Fl_Native_File_Chooser* fileChooser = nullptr;
    // Widget that holds the current project UI.
    Fl_Box* container = nullptr;
    NewDialog* newDlg = nullptr;
    AudioSettingsDialog* audioSettingsDlg = nullptr;
    MidiSettingsDialog* midiSettingsDlg = nullptr;
    AddTrackDialog* addTrackDlg = nullptr;
    // Stores menu item labels to prevent trash characters (eg: ^$¨)
    // when updating labels.
    std::map<Fl_Menu_Item*, std::string> menuItemLabels;
    std::string message;
    Project::Model* projectModel = nullptr;
    Project::Controller* projectCtrl = nullptr;
    Project::View* projectView = nullptr;
    Audio::Engine* audioEngine = nullptr;
    Midi::Engine* midiEngine = nullptr;
    Transport* transport = nullptr;
    TransportBar* transportBar = nullptr;
    MidiScheduler* midiScheduler = nullptr;

    void initAudioBackend();
    void initAudioDevices();

    public:

        Application(int w, int h, const char* l, int argc, char* argv[]);
        ~Application();

        void createMenu();
        const char* untitledDefault();
        int isFileExist(const char* filename);
        std::string getMessage() { return message; }
        void setMessage(std::string msg) { message = msg; }

        // Menu actions.
        void onMenuFile(FileID id);
        void onNew();
        void onOpen();
        void onClose();
        void onSave();
        void onSaveAs();
        void onQuit();
        void onMenuEdit(EditID id);
        void onCopy();
        void onPaste();
        void onCut();
        void onUndo();
        void onRedo();
        void onDelete();
        void onMenuTrack(TrackID id);
        void onAdd();
        void onRemove();
        void onMenuSettings(SettingsID id);
        void onAudioSettings();
        void onMidiSettings();

        void openFile(const char* filename);
        void createProject();
        Project::Model* getProject() { return projectModel; }
        Audio::Engine& getAudioEngine() { return *audioEngine; }
        void initAudioSystem();
        Midi::Engine& getMidiEngine() { return *midiEngine; }
        Transport& getTransport() { return *transport; }
        MidiScheduler& getMidiScheduler() { return *midiScheduler; }
        void initMidiSystem();
        std::string escapeMenuText(const std::string& input);
        void activateMenuItem(MenuItemID menuId);
        void deactivateMenuItem(MenuItemID menuId);

        // Call back functions.
        static void noEscapeKey_cb(Fl_Widget* w, void* data);
};

#endif
