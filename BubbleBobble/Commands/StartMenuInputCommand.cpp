#include "StartMenuInputCommand.h"

#include "../Components/StartMenuComponent.h"

StartMenuInputCommand::StartMenuInputCommand(StartMenuComponent* startMenu, StartMenuInputAction action)
	: m_StartMenu{ startMenu }
	, m_Action{ action }
{
}

void StartMenuInputCommand::Execute(bool notfirstExecute)
{
	if (m_StartMenu == nullptr)
		return;

	if (m_Action == StartMenuInputAction::Confirm && notfirstExecute)
		return;

	switch (m_Action)
	{
	case StartMenuInputAction::PreviousOption:
		if (!notfirstExecute)
			m_StartMenu->PreviousOption();
		break;
	case StartMenuInputAction::NextOption:
		if (!notfirstExecute)
			m_StartMenu->NextOption();
		break;
	case StartMenuInputAction::Confirm:
		m_StartMenu->Confirm();
		break;
	}
}
