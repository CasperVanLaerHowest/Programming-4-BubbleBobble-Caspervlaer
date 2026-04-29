#pragma once
#include "IAudioService.h"

class NullAudioService : public IAudioService {
public:
    void PlaySound(const std::string&) override {}  // No-op
    void StopSound(const std::string&) override {}
    void SetVolume(float) override {}
    void PlayMusic(const std::string&, bool) override {}
    void StopMusic() override {}
};