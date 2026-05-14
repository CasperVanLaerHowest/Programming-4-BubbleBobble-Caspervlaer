#include "FacingComponent.h"

FacingComponent::FacingComponent(dae::GameObject* owner)
	: Component(owner)
{
}

void FacingComponent::SetFacingDirection(FacingDirection direction)
{
	m_FacingDirection = direction;
}

void FacingComponent::SetFacingFromHorizontalMovement(float horizontalMovement)
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

FacingDirection FacingComponent::GetFacingDirection() const
{
	return m_FacingDirection;
}

glm::vec2 FacingComponent::GetFacingVector() const
{
	return IsFacingRight() ? glm::vec2{ 1.f, 0.f } : glm::vec2{ -1.f, 0.f };
}

bool FacingComponent::IsFacingRight() const
{
	return m_FacingDirection == FacingDirection::Right;
}
