#include <cmath>
#include "WalkState.h"
#include "GameObject.h"
#include "../Components/AnimationComponent.h"
#include "../Components/PhysicsComponent.h"
#include "IdleState.h"

void WalkState::Enter()
{
	m_pOwner->GetComponent<AnimationComponent>()->PlayAnimation("Walk");

	auto* physics = m_pOwner->GetComponent<PhysicsComponent>();
	auto* animation = m_pOwner->GetComponent<AnimationComponent>();
	if (physics && animation)
	{
		const auto& velocity = physics->GetVelocity();
		if (std::abs(velocity.x) > 0.01f)
		{
			animation->SetFlipHorizontal(velocity.x > 0.f);
		}
	}
}

std::unique_ptr<BaseState> WalkState::Update(float)
{
	auto* physics = m_pOwner->GetComponent<PhysicsComponent>();
	if (physics == nullptr)
	{
		return nullptr;
	}

	const auto& velocity = physics->GetVelocity();
	if (std::abs(velocity.x) >= 0.01f)
	{
		if (auto* animation = m_pOwner->GetComponent<AnimationComponent>())
		{
			animation->SetFlipHorizontal(velocity.x > 0.f);
		}

		return nullptr;
	}

	return std::make_unique<IdleState>(m_pOwner);
}
