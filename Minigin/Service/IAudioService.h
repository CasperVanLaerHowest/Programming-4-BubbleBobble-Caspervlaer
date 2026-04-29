#pragma once
#include <string>

class IAudioService {
public:
    virtual ~IAudioService() = default;
    virtual void PlaySound(const std::string& soundId) = 0;
    virtual void StopSound(const std::string& soundId) = 0;
    virtual void SetVolume(float volume) = 0;
    virtual void PlayMusic(const std::string& trackId, bool loop = true) = 0;
    virtual void StopMusic() = 0;
};