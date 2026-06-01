#include "../../main.h"


void Application::onAudioSettings()
{
    if (audioSettingsDlg == nullptr) {
        audioSettingsDlg = new AudioSettingsDialog(x() + MODAL_WND_POS, y() + MODAL_WND_POS,
                              XLARGE_SPACE + MEDIUM_SPACE, LARGE_SPACE + MEDIUM_SPACE,
                              "Audio Settings", *this);
    }

    if (audioSettingsDlg->runModal() == DIALOG_OK) {
        AppConfig& config = loadConfig();
        config.audio.backend = audioSettingsDlg->getBackend().text();
        config.audio.outputDevice = audioSettingsDlg->getOutput().text();
        config.audio.inputDevice = audioSettingsDlg->getInput().text();
        saveConfig();
    }
}

void Application::onMidiSettings()
{
    if (midiSettingsDlg == nullptr) {
        midiSettingsDlg = new MidiSettingsDialog(x() + MODAL_WND_POS, y() + MODAL_WND_POS,
                              XLARGE_SPACE + MEDIUM_SPACE, LARGE_SPACE + MEDIUM_SPACE,
                              "MIDI Settings", *this);
    }

    if (midiSettingsDlg->runModal() == DIALOG_OK) {

    }
}
