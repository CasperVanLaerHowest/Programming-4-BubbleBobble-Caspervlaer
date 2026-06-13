#pragma once
#include "Command.h"

enum class GameMode
{
	SinglePlayer,
	CoOp,
	Versus
};

class StartGameCommand final : public Command
{
public:
	using StartGameCallback = void(*)(GameMode);

	StartGameCommand(GameMode gameMode, StartGameCallback startGameCallback);
	void Execute(bool notfirstExecute) override;

private:
	GameMode m_GameMode{ GameMode::SinglePlayer };
	StartGameCallback m_StartGameCallback{};
};
