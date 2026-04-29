#include "AudioLocator.h"

IAudioService* AudioLocator::_service = nullptr;
NullAudioService AudioLocator::_nullService;