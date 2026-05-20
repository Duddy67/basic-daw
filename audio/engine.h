#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <vector>
#include <atomic>
#include "../main.h"
#include "../../libraries/miniaudio.h"

class Application;

namespace Audio {

    class Engine {

        // Structure that holds the backend data.
        struct BackendInfo {
            std::string name;
            ma_backend backend;
            bool isDefault;
        };

        // Structure that holds the device data.
        struct DeviceInfo {
            std::string name;
            ma_device_id id;
            bool isDefault;
        };

        Application& application;
        ma_context context;
        ma_device outputDevice;
        ma_device inputDevice;
        ma_device duplexDevice;
        ma_device_id outputDeviceID = {0};
        ma_device_id inputDeviceID = {0};
        ma_device_id duplexDeviceID = {0};
        // Flags to keep track of object state.
        bool contextInitialized = false;
        bool outputDeviceInitialized = false;
        bool inputDeviceInitialized = false;
        bool duplexDeviceInitialized = false;
        const ma_format defaultOutputFormat = ma_format_f32;
        const ma_uint32 defaultOutputSampleRate = 44100;
        void initializeOutputDevice();
        void initializeInputDevice();
        void initializeDuplexDevice();

        std::string backendToString(ma_backend backend);
        std::vector<DeviceInfo> getDevices(ma_device_type deviceType);
        bool isBackendAvailable(ma_backend backend);
        static void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frameCount);

      public:

          Engine(Application& app);
          ~Engine();

          void startPlayback() { ma_device_start(&outputDevice); }
          void stopPlayback()  { ma_device_stop(&outputDevice); }
          void startCapture() { ma_device_start(&inputDevice); }
          void stopCapture()  { ma_device_stop(&inputDevice); }
          void startDuplex() { ma_device_start(&duplexDevice); }
          void stopDuplex()  { ma_device_stop(&duplexDevice); }
          void uninitContext();
          void uninitOutput();
          void uninitInput();
          void uninitDuplex();
          bool isDeviceDuplex(const char *name);
          std::string currentBackend();
          std::string currentOutput();
          void printAllDevices();

          // Getters.
          std::vector<BackendInfo> getBackends();
          std::vector<DeviceInfo> getOutputDevices();
          std::vector<DeviceInfo> getInputDevices();
          std::vector<DeviceInfo> getDuplexDevices();
          bool isContextInitialized() { return contextInitialized; }
          ma_format getDefaultOutputFormat() const { return defaultOutputFormat; }
          ma_uint32 getDefaultOutputSampleRate() const { return defaultOutputSampleRate; }

          // Setters.
          void setBackend(const char *name);
          void setOutputDevice(const char *name = nullptr);
          void setInputDevice(const char *name = nullptr);
          void setDuplexDevice(const char *name = nullptr);

          void createTrack();
          void removeTrack();
    };
}

#endif // AUDIO_ENGINE_H

