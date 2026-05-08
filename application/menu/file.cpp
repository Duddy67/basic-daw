#include "../../main.h"


void Application::onNew()
{
    if (newDlg == nullptr) {
        newDlg = new NewDialog(x() + MODAL_WND_POS, y() + MODAL_WND_POS, XLARGE_SPACE, LARGE_SPACE, "New project");
    }

    if (newDlg->runModal() == DIALOG_OK) {
        // Add new file logic here...
        createProject();
    }
}

/*
 * Handle an 'Open' request from the menu.
 */
void Application::onOpen()
{
    fileChooser->title("Open file");
    // Only picks files that exist.
    fileChooser->type(Fl_Native_File_Chooser::BROWSE_FILE);     

    switch (fileChooser->show()) {
        case -1: break; // Error
        case  1: break; // Cancel
        default:        // Choice (ie: 0)
            fileChooser->preset_file(fileChooser->filename());
            openFile(fileChooser->filename());
        break;
    }
}

void Application::onClose()
{
    // Add close file logic here...
}

/*
 * Save the file, create the file if it doesn't exist
 * and save something in it.
 */
void Application::onSave()
{
    const char* filename = fileChooser->filename();
    printf("Saving '%s'\n", filename);

    if (!isFileExist(filename)) {
        // Create file if it doesn't exist.
        FILE *fp = fl_fopen(filename, "w");               

        if (fp) {
            // A real app would do something useful here.
            fprintf(fp, "Hello world.\n");
            fclose(fp);
        }
        else {
            fl_message("Error: %s: %s", filename, strerror(errno));
        }
    }
    else {
        // A real app would do something useful here.
    }
}

/*
 * Handle a 'Save as' request from the menu.
 */
void Application::onSaveAs()
{
    fileChooser->title("Save As");
    // Need this if file doesn't exist yet.
    fileChooser->type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);    

    switch (fileChooser->show()) {
        case -1: break;  // Error
        case  1: break;  // Cancel
        default:         // Choice (ie: 0)
            fileChooser->preset_file(fileChooser->filename());
            onSave();
        break;
    }
}

void Application::onQuit()
{
    // Add quit logic here...
    exit(0);
}

//  etc...
