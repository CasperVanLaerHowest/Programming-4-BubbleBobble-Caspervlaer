#include "PlayerFacingComponent.h"

PlayerFacingComponent::PlayerFacingComponent(dae::GameObject* owner)
	: Component(owner)
{
}

void PlayerFacingComponent::SetFacingDirection(FacingDirection direction)
{
	m_FacingDirection = direction;
}

void PlayerFacingComponent::SetFacingFromHorizontalMovement(float horizontalMovement)
{
	if (horizontalMovement < 0.f)
	{
		SetFacingDirection(FacingDirection::Left);
	}
	else if (horizontalMovement > 0.f)
	{
		SetFacingDirection(FacingDirection::Right);
	}
}

FacingDirection PlayerFacingComponent::GetFacingDirection() const
{
	return m_FacingDirection;
}

glm::vec2 PlayerFacingComponent::GetFacingVector() const
{
	return IsFacingRight() ? glm::vec2{ 1.f, 0.f } : glm::vec2{ -1.f, 0.f };
}

bool PlayerFacingComponent::IsFacingRight() const
{
	return m_FacingDirection == FacingDirection::Right;
}
