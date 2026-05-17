#include "main.h"


Application::Application(int w, int h, const char *l, int argc, char *argv[]) : Fl_Double_Window(w, h, l)
{
    box(FL_DOWN_BOX);
    color((Fl_Color) FL_INACTIVE_COLOR);

    // Create and build the menu.
    menu = new Fl_Menu_Bar(0, 0, w, SMALL_SPACE);
    menu->box(FL_THIN_UP_BOX);
    createMenu();
    menu->textsize(TEXT_SIZE);

    // Set menu item pointers.
    // Note: It's much easier to access menu items later than to rely on the find_item function.
    undoMenuItem = (Fl_Menu_Item *)menu->find_item(MenuLabels[MenuItemID::EDIT_UNDO].c_str());
    undoMenuItem->deactivate();
    redoMenuItem = (Fl_Menu_Item *)menu->find_item(MenuLabels[MenuItemID::EDIT_REDO].c_str());
    redoMenuItem->deactivate();

    toolbar = new Fl_Group(0, SMALL_SPACE, w, SMALL_SPACE);
    toolbar->box(FL_FLAT_BOX);

        // Other widgets go here...

    toolbar->end();

    // Add an invisible box to absorb resizing
    // It must be placed over the remaining space

    // Matches group size (ie: toolbar).
    container = new Fl_Box(0, SMALL_SPACE * 2, w, h - (SMALL_SPACE * 2)); 
    // Make it invisible.
    container->hide(); 
    // Set this box as the resizable area.
    resizable(container); 
    // Prevent toolbar (and its children) from being resized.
    toolbar->resizable(nullptr);

    //projectModel = new Project::Model(*this);
    //projectCtrl = new Project::Controller(*this, *projectModel);
    //projectView = new Project::View(0, SMALL_SPACE * 2, Fl::w(), Fl::h() - (SMALL_SPACE * 2), *projectCtrl);

    // Stop adding children to the main window.
    end();

    // Initialize the file chooser
    fileChooser = new Fl_Native_File_Chooser();
    // Sets the current filename filter patterns (ie: extension).
    fileChooser->filter("Text\t*.txt\n");
    // Sets the default filename for the chooser.
    fileChooser->preset_file(untitledDefault());

    //size_range(w, h, w, h);
    show();

    // Start application in maximized state.
    //maximize();

    this->callback(noEscapeKey_cb, this);
}

Application::~Application() 
{
    delete projectModel;
    delete projectCtrl;
    delete audioEngine;
    delete fileChooser;
    delete newDlg;
    delete addTrackDlg;
    // Don't delete menu, toolbar, container - FLTK manages these
}

int main(int argc, char *argv[])
{
    // Set the application to screen size (ie: Fl::w() and Fl::h()).
    //Application app(Fl::w(), Fl::h(), "Basic Application", argc, argv);
    Application app(900, 600, "Basic DAW", argc, argv);
    /*Application* app = new Application(900, 600, "Basic DAW", argc, argv);
    app->show();
    int result = Fl::run();
    delete app;
    return result;*/

    return Fl::run();
}
