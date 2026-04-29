#include "AudioService.h"
#include <SDL3_mixer/SDL_mixer.h> 
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_audio.h>
#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <variant>

// Define possible audio commands
struct PlaySoundCommand { std::string id; };
struct StopSoundCommand { std::string id; };
struct PlayMusicCommand { std::string id; bool loop; };
struct StopMusicCommand {};
struct SetVolumeCommand { float volume; };

using AudioCommand = std::variant<PlaySoundCommand, StopSoundCommand, PlayMusicCommand, StopMusicCommand, SetVolumeCommand>;

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
        
        if (m_Mixer) {
            m_MusicTrack = MIX_CreateTrack(m_Mixer);
        }

        // Start the worker thread
        m_AudioThread = std::thread(&AudioServiceImpl::ProcessCommands, this);
    }

    ~AudioServiceImpl() {
        // Signal the thread to stop and wait for it
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_StopThread = true;
        }
        m_Condition.notify_one();

        if (m_AudioThread.joinable()) {
            m_AudioThread.join();
        }

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
        EnqueueCommand(PlaySoundCommand{soundId});
    }

    void StopSound(const std::string& soundId) {
        EnqueueCommand(StopSoundCommand{soundId});
    }

    void SetVolume(float volume) {
        EnqueueCommand(SetVolumeCommand{volume});
    }

    void PlayMusic(const std::string& trackId, bool loop) {
        EnqueueCommand(PlayMusicCommand{trackId, loop});
    }

    void StopMusic() {
        EnqueueCommand(StopMusicCommand{});
    }

private:
    MIX_Mixer* m_Mixer = nullptr;
    MIX_Track* m_MusicTrack = nullptr;
    std::unordered_map<std::string, MIX_Audio*> m_Sounds;
    std::unordered_map<std::string, MIX_Audio*> m_Music;

    // Threading variables
    std::thread m_AudioThread;
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    std::queue<AudioCommand> m_EventQueue;
    bool m_StopThread = false;

    void EnqueueCommand(AudioCommand command) {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_EventQueue.push(std::move(command));
        }
        m_Condition.notify_one();
    }

    void ProcessCommands() {
        while (true) {
            AudioCommand command;
            {
                std::unique_lock<std::mutex> lock(m_Mutex);
                m_Condition.wait(lock, [this] { return m_StopThread || !m_EventQueue.empty(); });

                if (m_StopThread && m_EventQueue.empty()) {
                    break;
                }

                command = std::move(m_EventQueue.front());
                m_EventQueue.pop();
            }

            // Execute command
            std::visit([this](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, PlaySoundCommand>) {
                    HandlePlaySound(arg.id);
                } else if constexpr (std::is_same_v<T, StopSoundCommand>) {
                    HandleStopSound(arg.id);
                } else if constexpr (std::is_same_v<T, PlayMusicCommand>) {
                    HandlePlayMusic(arg.id, arg.loop);
                } else if constexpr (std::is_same_v<T, StopMusicCommand>) {
                    HandleStopMusic();
                } else if constexpr (std::is_same_v<T, SetVolumeCommand>) {
                    HandleSetVolume(arg.volume);
                }
            }, command);
        }
    }

    void HandlePlaySound(const std::string& soundId) {
        if (!m_Mixer) return;

        if (m_Sounds.find(soundId) == m_Sounds.end()) {
            MIX_Audio* chunk = MIX_LoadAudio(m_Mixer, soundId.c_str(), true);
            if (chunk == nullptr) {
                std::cerr << "Failed to load sound: " << soundId << " Error: " << SDL_GetError() << "\n";
                return;
            }
            m_Sounds[soundId] = chunk;
        }

        MIX_PlayAudio(m_Mixer, m_Sounds[soundId]);
    }

    void HandleStopSound(const std::string& soundId) {
        std::cout << "StopSound called for: " << soundId << " (Requires explicit Track management to halt individually)\n";
    }

    void HandleSetVolume(float volume) {
        if (!m_Mixer) return;
        MIX_SetMixerGain(m_Mixer, volume);
    }

    void HandlePlayMusic(const std::string& trackId, bool loop) {
        if (!m_Mixer || !m_MusicTrack) return;

        if (m_Music.find(trackId) == m_Music.end()) {
            MIX_Audio* music = MIX_LoadAudio(m_Mixer, trackId.c_str(), false);
            if (music == nullptr) {
                std::cerr << "Failed to load music: " << trackId << " Error: " << SDL_GetError() << "\n";
                return;
            }
            m_Music[trackId] = music;
        }

        MIX_SetTrackAudio(m_MusicTrack, m_Music[trackId]);
        MIX_SetTrackLoops(m_MusicTrack, loop ? -1 : 0);
        MIX_PlayTrack(m_MusicTrack, 0); 
    }

    void HandleStopMusic() {
        if (m_MusicTrack) {
            MIX_StopTrack(m_MusicTrack, 0);
        }
    }
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