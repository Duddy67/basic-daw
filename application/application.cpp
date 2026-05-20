#include "../main.h"
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
        projectView = new Project::View(0, SMALL_SPACE * 2, Fl::w(), Fl::h() - (SMALL_SPACE * 2), *projectCtrl);

        // Make projectView child of Application.
        add(projectView);
        redraw();
    }
}

void Application::saveConfig(const AppConfig& config, const std::string& filename)
{
    json j;
    j["backend"] = config.backend;
    j["outputDevice"] = config.outputDevice;
    j["inputDevice"] = config.inputDevice;
    //j["volume"] = config.volume;

    std::ofstream file(filename);
    file << j.dump(4); // Pretty print with 4 spaces indentation
    std::cout << "Configuration saved to " << filename << std::endl;

}

Application::AppConfig Application::loadConfig(const std::string& filename)
{
    AppConfig config;
    std::ifstream file(filename);

    // If no config file is found, create it.
    if (!file.is_open()) {
        config.backend = "";
        config.outputDevice = "";
        config.inputDevice = "";
        //config.volume = "0";
        this->saveConfig(config, filename);
        return config;
    }

    try {
        json j;
        file >> j;

        config.backend = j.value("backend", "");
        config.outputDevice = j.value("outputDevice", "");
        config.inputDevice = j.value("inputDevice", "");
        //config.volume = j.value("volume", "0");
    }
    catch (const json::exception& e) {
        setMessage("Error parsing config: " + std::string(e.what()));
    }

    return config;
}

