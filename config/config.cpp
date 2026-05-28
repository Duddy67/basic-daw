#include "config.h"

/*
 * Sets config from json file.
 */
void from_json(const json& j, AppConfig& c)
{
    j.at("audio").at("backend").get_to(c.audio.backend);
    j.at("audio").at("output_device").get_to(c.audio.outputDevice);
    j.at("audio").at("input_device").get_to(c.audio.inputDevice);

    j.at("midi").at("device").get_to(c.midi.device);
    j.at("midi").at("output_port").get_to(c.midi.outputPort);
    j.at("midi").at("input_port").get_to(c.midi.inputPort);
}

/*
 * Writes config to json file.
 */
void to_json(json& j, const AppConfig& c)
{
    j["audio"]["backend"] = c.audio.backend;
    j["audio"]["output_device"] = c.audio.outputDevice;
    j["audio"]["input_device"] = c.audio.inputDevice;

    j["midi"]["device"] = c.midi.device;
    j["midi"]["output_port"] = c.midi.outputPort;
    j["midi"]["input_port"] = c.midi.inputPort;
}

AppConfig& loadConfig()
{
    // Called once (ie: singleton).
    static AppConfig config = [] {
        std::ifstream file(CONFIG_FILENAME);

        // If no config file is found, create it.
        if (!file.is_open()) {
            std::ofstream file(CONFIG_FILENAME);
            json jsonData;
            // Write default config.
            to_json(jsonData, AppConfig{});
            // Pretty print with 4 spaces indentation.
            file << jsonData.dump(4); 

            // Return default config.
            return AppConfig{};
        }

        try {
            json jsonData;
            file >> jsonData;

            // Call from_json function (ADL => Argument-Dependent Lookup).
            return jsonData.get<AppConfig>();
        }
        catch (const json::exception& e) {
            std::cerr << "Error: Cannot parsing config: " << std::string(e.what()) << std::endl;
            return AppConfig{};
        }
    }();

    return config;
}

void saveConfig()
{
    AppConfig& config = loadConfig();
    json jsonData;
    to_json(jsonData, config);

    std::ofstream file(CONFIG_FILENAME);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot save config to " << CONFIG_FILENAME << std::endl;
        return;
    }

    // Pretty print with 4 spaces
    file << jsonData.dump(4);  
    std::cout << "Config saved to: " << CONFIG_FILENAME << std::endl;
}

