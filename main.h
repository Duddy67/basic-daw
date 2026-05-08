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
#include "dialogs/new.h"
#include "dialogs/add_track.h"
#include "project/model.h"
#include "project/controller.h"
#include "project/view.h"
#include "audio/engine.h"

// Forward class declarations.
namespace Project {
    class Controller;
    class View;
}

namespace Audio {
    class Engine;
}

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
    AddTrackDialog* addTrackDlg = nullptr;
    // Stores menu item labels to prevent trash characters (eg: ^$¨)
    // when updating labels.
    std::map<Fl_Menu_Item*, std::string> menuItemLabels;
    std::string message;
    Project::Model* projectModel = nullptr;
    Project::Controller* projectCtrl = nullptr;
    Project::View* projectView = nullptr;
    Audio::Engine* audioEngine = nullptr;

    public:

        Application(int w, int h, const char* l, int argc, char* argv[]);

        void createMenu();
        const char* untitledDefault();
        int isFileExist(const char* filename);
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

        void openFile(const char* filename);
        void createProject();
        Audio::Engine& getAudioEngine() { return *audioEngine; }

        // Call back functions.
        static void noEscapeKey_cb(Fl_Widget* w, void* data);
};

#endif
