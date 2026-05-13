#include <cmath>
#include "Idlestate.h"
#include "GameObject.h"
#include "WalkState.h"
#include "../Components/AnimationComponent.h"
#include "../Components/PhysicsComponent.h"

void IdleState::Enter()
{
	m_pOwner->GetComponent<AnimationComponent>()->PlayAnimation("Idle");
}

std::unique_ptr<BaseState> IdleState::Update(float)
{
	auto* physics = m_pOwner->GetComponent<PhysicsComponent>();
	if (physics == nullptr)
	{
		return nullptr;
	}

	const auto& velocity = physics->GetVelocity();
	if (std::abs(velocity.x) <= 0.01f)
	{
		return nullptr;
	}

	if (auto* animation = m_pOwner->GetComponent<AnimationComponent>())
	{
		animation->SetFlipHorizontal(velocity.x > 0.f);
	}

	return std::make_unique<WalkState>(m_pOwner);
}
