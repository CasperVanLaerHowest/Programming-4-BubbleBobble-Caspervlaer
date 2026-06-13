#include "AudioObserver.h"

#include "Service/AudioLocator.h"

AudioObserver::AudioObserver(dae::GameObject* owner)
	: Component(owner)
{
}

void AudioObserver::OnNotify(dae::GameObject&, dae::Event event)
{
	if (event == dae::Event::TAKE_DAMAGE)
	{
		AudioLocator::GetAudio().PlaySound("Data/Sound/hurt.wav");
	}
}
