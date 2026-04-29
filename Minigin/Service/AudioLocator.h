#include <memory>
#include "IAudioService.h"
#include "NullAudioService.h"

class AudioLocator {
public:
    // Must be called at startup before any usage
    static void Initialize() {
        _service = &_nullService;
    }

    static IAudioService& GetAudio() {
        return *_service;
    }

    static void Provide(IAudioService* service) {
        // If null is explicitly provided, fall back to null service
        _service = (service != nullptr) ? service : &_nullService;
    }

private:
    static IAudioService* _service;
    static NullAudioService _nullService;
};