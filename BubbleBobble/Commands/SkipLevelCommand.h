#pragma once

#include "Command.h"

class LevelFlowComponent;

class SkipLevelCommand final : public Command
{
public:
	explicit SkipLevelCommand(LevelFlowComponent* levelFlow);
	void Execute(bool notfirstExecute) override;

private:
	LevelFlowComponent* m_LevelFlow{};
};
