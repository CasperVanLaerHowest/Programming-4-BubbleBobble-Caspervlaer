#include "SkipLevelCommand.h"

#include "../Components/LevelFlowComponent.h"

SkipLevelCommand::SkipLevelCommand(LevelFlowComponent* levelFlow)
	: m_LevelFlow{ levelFlow }
{
}

void SkipLevelCommand::Execute(bool notfirstExecute)
{
	if (notfirstExecute || m_LevelFlow == nullptr)
		return;

	m_LevelFlow->SkipLevel();
}
