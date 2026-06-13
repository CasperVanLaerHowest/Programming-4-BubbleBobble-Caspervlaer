#include "MaitaPlayerComponent.h"

#include "GameObject.h"
#include "PhysicsComponent.h"
#include "PlayerStateComponent.h"
#include "TransformComponent.h"

MaitaPlayerComponent::MaitaPlayerComponent(dae::GameObject* owner)
	: Component(owner)
{
}

void MaitaPlayerComponent::TrapInBubble()
{
	m_IsTrapped = true;

	if (auto* physics = GetOwner()->GetComponent<PhysicsComponent>())
	{
		physics->SetVelocity({ 0.f, 0.f });
		physics->SetGravity(0.f);
	}

	if (auto* transform = GetOwner()->GetComponent<dae::TransformComponent>())
	{
		transform->SetLocalPosition(-10000.f, -10000.f, 0.f);
	}
}

void MaitaPlayerComponent::ReleaseFromBubble(const glm::vec2& position)
{
	m_IsTrapped = false;

	if (auto* transform = GetOwner()->GetComponent<dae::TransformComponent>())
	{
		transform->SetLocalPosition(position.x, position.y, 0.f);
	}

	if (auto* physics = GetOwner()->GetComponent<PhysicsComponent>())
	{
		physics->SetVelocity({ 0.f, 0.f });
		physics->ResetGravity();
	}
}

void MaitaPlayerComponent::PopFromBubble(const glm::vec2& position)
{
	ReleaseFromBubble(position);

	if (auto* playerState = GetOwner()->GetComponent<PlayerStateComponent>())
	{
		playerState->TakeHit();
	}
}
