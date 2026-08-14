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
#include "core/engine.h"
#include "audio/processor.h"
#include "core/transport.h"
#include "midi/scheduler.h"
#include "widgets/transport_bar.h"
#include "json.hpp"

using json = nlohmann::json;

// Forward class declarations.

namespace Project {
    class Controller;
    class View;
}

namespace Core {
    class Engine;
}

namespace Audio {
    class Processor;
}

namespace Midi {
    class Scheduler;
}

class Transport;


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
    Core::Engine* coreEngine = nullptr;
    Audio::Processor* audioProcessor = nullptr;
    Transport* transport = nullptr;
    TransportBar* transportBar = nullptr;
    Midi::Scheduler* midiScheduler = nullptr;

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
        void onMenuSettings(DataType dataType);
        void onAudioSettings();
        void onMidiSettings();

        void openFile(const char* filename);
        void createProject();
        Project::Model* getProject() { return projectModel; }
        Core::Engine& getCoreEngine() { return *coreEngine; }
        Audio::Processor& getAudioProcessor() { return *audioProcessor; }
        //void initAudioSystem();
        void initJackClient();
        Transport& getTransport() { return *transport; }
        Midi::Scheduler& getMidiScheduler() { return *midiScheduler; }
        std::string escapeMenuText(const std::string& input);
        void activateMenuItem(MenuItemID menuId);
        void deactivateMenuItem(MenuItemID menuId);

        // Call back functions.
        static void noEscapeKey_cb(Fl_Widget* w, void* data);
};

#endif
