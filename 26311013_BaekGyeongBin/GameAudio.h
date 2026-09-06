#pragma once
#include <windows.h>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#pragma comment(lib, "ole32.lib")

// Controls only the default audio session belonging to this process.
class GameAudio {
public:
    bool Init() {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        ownsCom = SUCCEEDED(hr);
        if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) return false;
        IMMDeviceEnumerator* enumerator = nullptr;
        IMMDevice* device = nullptr;
        IAudioSessionManager* manager = nullptr;
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
            __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&enumerator));
        if (SUCCEEDED(hr)) hr = enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
        if (SUCCEEDED(hr)) hr = device->Activate(__uuidof(IAudioSessionManager), CLSCTX_ALL,
            nullptr, reinterpret_cast<void**>(&manager));
        if (SUCCEEDED(hr)) hr = manager->GetSimpleAudioVolume(nullptr, 0, &volume);
        if (manager) manager->Release();
        if (device) device->Release();
        if (enumerator) enumerator->Release();
        return SUCCEEDED(hr);
    }
    bool SetVolume(int percent) {
        return volume && SUCCEEDED(volume->SetMasterVolume(percent / 100.0f, nullptr));
    }
    void Destroy() {
        if (volume) { volume->Release(); volume = nullptr; }
        if (ownsCom) { CoUninitialize(); ownsCom = false; }
    }
private:
    ISimpleAudioVolume* volume = nullptr;
    bool ownsCom = false;
};
