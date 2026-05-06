#include "WalkState.h"
#include "GameObject.h"
#include "../Components/AnimationComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/PlayerStateComponent.h"
#include "IdleState.h"

void WalkState::Enter()
{
	m_pOwner->GetComponent<AnimationComponent>()->PlayAnimation("Walk");
}

void WalkState::Update(float)
{
	auto* physics = m_pOwner->GetComponent<PhysicsComponent>();
	if (physics == nullptr)
	{
		return;
	}

	const auto& velocity = physics->GetVelocity();
	if (std::abs(velocity.x) >= 0.01f)
	{
		return;
	}

	auto* stateComponent = m_pOwner->GetComponent<PlayerStateComponent>();
	if (stateComponent == nullptr)
	{
		return;
	}

	stateComponent->ChangeState(std::make_unique<IdleState>(m_pOwner));
}