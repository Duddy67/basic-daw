#include "../main.h"


void Application::createMenu()
{
    menu->add(MenuLabels[MenuItemID::FILE_SUB].c_str(), 0, 0, 0, FL_SUBMENU);
    // Use lambda function.
    menu->add(MenuLabels[MenuItemID::FILE_NEW].c_str(), 0, [](Fl_Widget* w, void* userData) { 
                                      Application* app = static_cast<Application*>(userData);
                                      app->onMenuFile(FileID::NEW);
                                  }, (void*) this);
    menu->add(MenuLabels[MenuItemID::FILE_OPEN].c_str(), 0, [](Fl_Widget* w, void* userData) { 
                                      Application* app = static_cast<Application*>(userData);
                                      app->onMenuFile(FileID::OPEN);
                                  }, (void*) this);
    menu->add(MenuLabels[MenuItemID::FILE_SAVE].c_str(), 0, [](Fl_Widget* w, void* userData) { 
                                      Application* app = static_cast<Application*>(userData);
                                      app->onMenuFile(FileID::SAVE);
                                  }, (void*) this);
    menu->add(MenuLabels[MenuItemID::FILE_SAVE_AS].c_str(), 0, [](Fl_Widget* w, void* userData) { 
                                      Application* app = static_cast<Application*>(userData);
                                      app->onMenuFile(FileID::SAVE_AS);
                                  }, (void*) this);
    menu->add(MenuLabels[MenuItemID::FILE_QUIT].c_str(), 0, [](Fl_Widget* w, void* userData) { 
                                      Application* app = static_cast<Application*>(userData);
                                      app->onMenuFile(FileID::QUIT);
                                  }, (void*) this);
    menu->add(MenuLabels[MenuItemID::EDIT_SUB].c_str(), 0, 0, 0, FL_SUBMENU);
    menu->add(MenuLabels[MenuItemID::EDIT_UNDO].c_str(), 0, [](Fl_Widget* w, void* userData) { 
                                      Application* app = static_cast<Application*>(userData);
                                      app->onMenuEdit(EditID::UNDO);
                                  }, (void*) this);
    menu->add(MenuLabels[MenuItemID::EDIT_REDO].c_str(), 0, [](Fl_Widget* w, void* userData) { 
                                      Application* app = static_cast<Application*>(userData);
                                      app->onMenuEdit(EditID::REDO);
                                  }, (void*) this);
    menu->add(MenuLabels[MenuItemID::EDIT_DELETE].c_str(), 0, [](Fl_Widget* w, void* userData) { 
                                      Application* app = static_cast<Application*>(userData);
                                      app->onMenuEdit(EditID::DELETE);
                                  }, (void*) this);
    menu->add(MenuLabels[MenuItemID::EDIT_COPY].c_str(), FL_CTRL + 'c',0, 0, 0);
    menu->add(MenuLabels[MenuItemID::EDIT_PASTE].c_str(), FL_CTRL + 'v',0, 0, FL_MENU_INACTIVE);
    menu->add(MenuLabels[MenuItemID::EDIT_CUT].c_str(), FL_CTRL + 'x',0, 0, 0);
    menu->add(MenuLabels[MenuItemID::TRACK_SUB].c_str(), 0, 0, 0, FL_SUBMENU);
    menu->add(MenuLabels[MenuItemID::TRACK_ADD].c_str(), 0, [](Fl_Widget* w, void* userData) { 
                                      Application* app = static_cast<Application*>(userData);
                                      app->onMenuTrack(TrackID::ADD);
                                  }, (void*) this);
    menu->add(MenuLabels[MenuItemID::TRACK_REMOVE].c_str(), 0, [](Fl_Widget* w, void* userData) { 
                                      Application* app = static_cast<Application*>(userData);
                                      app->onMenuTrack(TrackID::REMOVE);
                                  }, (void*) this);
    menu->add("Help", 0, 0, 0, FL_SUBMENU);
    menu->add("Help/Index", 0, 0, 0, 0);
    menu->add("Help/About", 0, 0, 0, 0);
    // etc...

    return;
}

/*
 * Maps the file menu item clicked to the according functions.
 */
void Application::onMenuFile(FileID id)
{
    switch (id) {
        case FileID::NEW:
            onNew();
            break;

        case FileID::OPEN:
            onOpen();
            break;

        case FileID::CLOSE:
            onClose();
            break;

        case FileID::SAVE:
            onSave();
            break;

        case FileID::SAVE_AS:
            onSaveAs();
            break;

        case FileID::QUIT:
            onQuit();
            break;
    }
}

/*
 * Maps the edit menu item clicked to the according functions.
 */
void Application::onMenuEdit(EditID id)
{
    switch (id) {
        case EditID::DELETE:
            onDelete();
            break;

        case EditID::COPY:
            onCopy();
            break;

        case EditID::PASTE:
            onPaste();
            break;

        case EditID::CUT:
            onCut();
            break;

        case EditID::UNDO:
            onUndo();
            break;

        case EditID::REDO:
            onRedo();
            break;

        case EditID::NONE:
            return;
    }
}

/*
 * Maps the track menu item clicked to the according functions.
 */
void Application::onMenuTrack(TrackID id)
{
    switch (id) {
        case TrackID::ADD:
            onAdd();
            break;

        case TrackID::REMOVE:
            onRemove();
            break;
    }
}
