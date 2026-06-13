#pragma once

#include "Command.h"

class HighScoreEntryComponent;

enum class HighScoreInputAction
{
	PreviousCharacter,
	NextCharacter,
	PreviousSlot,
	NextSlot,
	Confirm
};

class HighScoreInputCommand final : public Command
{
public:
	HighScoreInputCommand(HighScoreEntryComponent* highScoreEntry, HighScoreInputAction action);
	void Execute(bool notfirstExecute) override;

private:
	HighScoreEntryComponent* m_HighScoreEntry{};
	HighScoreInputAction m_Action{};
};
