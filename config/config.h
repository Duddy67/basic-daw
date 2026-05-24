#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <iostream>
#include "../constants.h"
#include "../../libraries/json.hpp"

using json = nlohmann::json;


/*
 * The configurable parameters of the application.
 */
struct AppConfig {
    struct Audio {
        std::string backend = "";
        std::string outputDevice = "";
        std::string inputDevice = "";
    } audio;

    struct Midi {
        std::string port = "";
        std::string outputDevice = "";
        std::string inputDevice = "";
    } midi;
};

void from_json(const json& j, AppConfig& c);
void to_json(json& j, const AppConfig& c);

AppConfig& loadConfig();
void saveConfig();

#endif // CONFIG_H
