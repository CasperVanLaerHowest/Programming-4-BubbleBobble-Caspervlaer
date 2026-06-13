#include "PlayerSlotComponent.h"

PlayerSlotComponent::PlayerSlotComponent(dae::GameObject* owner, int playerIndex)
	: Component(owner)
	, m_PlayerIndex{ playerIndex }
{
}
