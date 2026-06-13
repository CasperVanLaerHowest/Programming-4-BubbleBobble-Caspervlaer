#pragma once

#include "Component.h"

class PlayerSlotComponent final : public dae::Component
{
public:
	PlayerSlotComponent(dae::GameObject* owner, int playerIndex);

	int GetPlayerIndex() const { return m_PlayerIndex; }

private:
	int m_PlayerIndex{};
};
