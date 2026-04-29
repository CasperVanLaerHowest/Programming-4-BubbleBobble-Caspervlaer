#include "AudioService.h"
#include <SDL3_mixer/SDL_mixer.h> 
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_audio.h>
#include <iostream>
#include <unordered_map>

class AudioService::AudioServiceImpl {
public:
    AudioServiceImpl() {
        // Initialize SDL_mixer for SDL3
        if (!MIX_Init()) {
            std::cerr << "SDL3_mixer could not initialize! Error: " << SDL_GetError() << "\n";
            return;
        }

        SDL_AudioSpec spec;
        spec.freq = 44100;
        spec.format = SDL_AUDIO_S16;
        spec.channels = 2;

        m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
        if (!m_Mixer) {
            std::cerr << "Failed to create mixer device! Error: " << SDL_GetError() << "\n";
        }
        
        // We dedicate a distinct track to behave like the 'music channel'
        if (m_Mixer) {
            m_MusicTrack = MIX_CreateTrack(m_Mixer);
        }
    }

    ~AudioServiceImpl() {
        if (m_MusicTrack) {
            MIX_DestroyTrack(m_MusicTrack);
        }

        // Clean up cached resources
        for (auto& pair : m_Sounds) {
            MIX_DestroyAudio(pair.second);
        }
        for (auto& pair : m_Music) {
            MIX_DestroyAudio(pair.second);
        }

        if (m_Mixer) {
            MIX_DestroyMixer(m_Mixer);
        }
        MIX_Quit();
    }

    void PlaySound(const std::string& soundId) {
        if (!m_Mixer) return;

        if (m_Sounds.find(soundId) == m_Sounds.end()) {
            // "true" here tells the mixer to predecode the sound to RAM for fast playback
            MIX_Audio* chunk = MIX_LoadAudio(m_Mixer, soundId.c_str(), true);
            if (chunk == nullptr) {
                std::cerr << "Failed to load sound: " << soundId << " Error: " << SDL_GetError() << "\n";
                return;
            }
            m_Sounds[soundId] = chunk;
        }

        // Fire and forget audio playback
        MIX_PlayAudio(m_Mixer, m_Sounds[soundId]);
    }

    void StopSound(const std::string& soundId) {
        // SDL3_mixer's MIX_PlayAudio handles fire-and-forget sounds that cannot individually be halted easily. 
        std::cout << "StopSound called for: " << soundId << " (Requires explicit Track management to halt individually)\n";
    }

    void SetVolume(float volume) {
        if (!m_Mixer) return;
        // Sets the master gain across the entire mixer (0.0f = silence, 1.0f = full)
        MIX_SetMixerGain(m_Mixer, volume);
    }

    void PlayMusic(const std::string& trackId, bool loop) {
        if (!m_Mixer || !m_MusicTrack) return;

        if (m_Music.find(trackId) == m_Music.end()) {
            // "false" here means the file decodes on the fly, saving lots of memory for long files
            MIX_Audio* music = MIX_LoadAudio(m_Mixer, trackId.c_str(), false);
            if (music == nullptr) {
                std::cerr << "Failed to load music: " << trackId << " Error: " << SDL_GetError() << "\n";
                return;
            }
            m_Music[trackId] = music;
        }

        MIX_SetTrackAudio(m_MusicTrack, m_Music[trackId]);
        MIX_SetTrackLoops(m_MusicTrack, loop ? -1 : 0);
        // 0 tells it to use default playing properties
        MIX_PlayTrack(m_MusicTrack, 0); 
    }

    void StopMusic() {
        if (m_MusicTrack) {
            MIX_StopTrack(m_MusicTrack, 0);
        }
    }

private:
    MIX_Mixer* m_Mixer = nullptr;
    MIX_Track* m_MusicTrack = nullptr;
    std::unordered_map<std::string, MIX_Audio*> m_Sounds;
    std::unordered_map<std::string, MIX_Audio*> m_Music;
};

// Map interface functions to pImpl
AudioService::AudioService() : pImpl(std::make_unique<AudioServiceImpl>()) {}
AudioService::~AudioService() = default;
AudioService::AudioService(AudioService&&) noexcept = default;
AudioService& AudioService::operator=(AudioService&&) noexcept = default;

void AudioService::PlaySound(const std::string& soundId) { pImpl->PlaySound(soundId); }
void AudioService::StopSound(const std::string& soundId) { pImpl->StopSound(soundId); }
void AudioService::SetVolume(float volume) { pImpl->SetVolume(volume); }
void AudioService::PlayMusic(const std::string& trackId, bool loop) { pImpl->PlayMusic(trackId, loop); }
void AudioService::StopMusic() { pImpl->StopMusic(); }