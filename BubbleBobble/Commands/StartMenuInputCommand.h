#pragma once

#include "Command.h"

class StartMenuComponent;

enum class StartMenuInputAction
{
	PreviousOption,
	NextOption,
	Confirm
};

class StartMenuInputCommand final : public Command
{
public:
	StartMenuInputCommand(StartMenuComponent* startMenu, StartMenuInputAction action);
	void Execute(bool notfirstExecute) override;

private:
	StartMenuComponent* m_StartMenu{};
	StartMenuInputAction m_Action{};
};
