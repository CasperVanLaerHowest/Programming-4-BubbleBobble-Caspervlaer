#include "MuteCommand.h"
#include "Service/AudioLocator.h"

void MuteCommand::Execute(bool notfirstExecute)
{
	if (notfirstExecute)
		return;

	m_IsMuted = !m_IsMuted;
	AudioLocator::GetAudio().SetVolume(m_IsMuted ? 0.f : 1.f);
}
