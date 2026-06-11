#include "../main.h"
#include "../constants.h"
#include <cstdlib>
#include <fstream>

/*
 * Open the given file.
 */
void Application::openFile(const char* filename)
{
    // A real app would do something useful here.
    printf("Open: '%s'\n", filename);
}

/*
 * Return an 'untitled' default pathname.
 */
const char* Application::untitledDefault()
{
    static char* filename = 0;

    if (!filename) {
        const char* home = getenv("HOME") ? getenv("HOME") : // Unix
        getenv("HOME_PATH") ? getenv("HOME_PATH") :          // Windows
        ".";                                                 // other

        filename = (char*)malloc(strlen(home) + 20);
        sprintf(filename, "%s/untitled.txt", home);
    }

    return(filename);
}

int Application::isFileExist(const char* filename) {
    FILE* fp = fl_fopen(filename, "r");

    if (fp) {
        fclose(fp);
        return(1);
    }
    else {
        return(0);
    }
}

void Application::createProject()
{
    if (projectModel == nullptr) {
        projectModel = new Project::Model(*this);
        projectCtrl = new Project::Controller(*this, *projectModel);
        projectView = new Project::View(0, (SMALL_SPACE * 2) + (TINY_SPACE * 2), Fl::w(), Fl::h() - (SMALL_SPACE * 2), *projectCtrl);

        // Make projectView child of Application.
        add(projectView);
        redraw();
    }
}

/*
 * Utility function which escapes characters considered as special by FLTK (ie: / & _).
 */
std::string Application::escapeMenuText(const std::string& input) {
    std::string result;

    for (char c : input) {
        if (c == '/' || c == '&' || c == '_') {
            result += '\\';  // FLTK uses backslash for escaping
            result += c;
        }
        else {
            result += c;
        }
    }

    return result;
}

void Application::activateMenuItem(MenuItemID menuId)
{
    Fl_Menu_Item *item = nullptr;

    if ((item = (Fl_Menu_Item*)menu->find_item(MenuLabels[menuId].c_str())) != nullptr) {
        item->activate();
    }
}

void Application::deactivateMenuItem(MenuItemID menuId)
{
    Fl_Menu_Item *item = nullptr;

    if ((item = (Fl_Menu_Item*)menu->find_item(MenuLabels[menuId].c_str())) != nullptr) {
        item->deactivate();
    }
}

