#include "StartGameCommand.h"

StartGameCommand::StartGameCommand(GameMode gameMode, StartGameCallback startGameCallback)
	: m_GameMode{ gameMode }
	, m_StartGameCallback{ startGameCallback }
{
}

void StartGameCommand::Execute(bool notfirstExecute)
{
	if (notfirstExecute || !m_StartGameCallback)
		return;

	m_StartGameCallback(m_GameMode);
}
