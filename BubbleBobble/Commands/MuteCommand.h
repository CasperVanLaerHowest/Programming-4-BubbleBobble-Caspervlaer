#pragma once

#include "Command.h"

class MuteCommand final : public Command
{
public:
	explicit MuteCommand() = default;
	void Execute(bool notfirstExecute) override;

private:
	bool m_IsMuted{ false };
};
