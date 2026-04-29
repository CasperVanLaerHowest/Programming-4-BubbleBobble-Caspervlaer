#pragma once
#include "IAudioService.h"
#include <memory>
#include <string>

class AudioService : public IAudioService {
public:
    AudioService();
    ~AudioService() override;

    // Rule of 5: Prevent copies, but allow moves since we manage a unique_ptr
    AudioService(const AudioService&) = delete;
    AudioService& operator=(const AudioService&) = delete;
    AudioService(AudioService&&) noexcept;
    AudioService& operator=(AudioService&&) noexcept;

    void PlaySound(const std::string& soundId) override;
    void StopSound(const std::string& soundId) override;
    void SetVolume(float volume) override;
    void PlayMusic(const std::string& trackId, bool loop) override;
    void StopMusic() override;

private:
    class AudioServiceImpl;
    std::unique_ptr<AudioServiceImpl> pImpl;
};