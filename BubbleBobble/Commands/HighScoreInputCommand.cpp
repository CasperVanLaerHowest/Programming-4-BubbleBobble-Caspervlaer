#include "HighScoreInputCommand.h"

#include "../Components/HighScoreEntryComponent.h"

HighScoreInputCommand::HighScoreInputCommand(HighScoreEntryComponent* highScoreEntry, HighScoreInputAction action)
	: m_HighScoreEntry{ highScoreEntry }
	, m_Action{ action }
{
}

void HighScoreInputCommand::Execute(bool notfirstExecute)
{
	if (notfirstExecute || m_HighScoreEntry == nullptr)
		return;

	switch (m_Action)
	{
	case HighScoreInputAction::PreviousCharacter:
		m_HighScoreEntry->PreviousCharacter();
		break;
	case HighScoreInputAction::NextCharacter:
		m_HighScoreEntry->NextCharacter();
		break;
	case HighScoreInputAction::PreviousSlot:
		m_HighScoreEntry->PreviousSlot();
		break;
	case HighScoreInputAction::NextSlot:
		m_HighScoreEntry->NextSlot();
		break;
	case HighScoreInputAction::Confirm:
		m_HighScoreEntry->Confirm();
		break;
	}
}
