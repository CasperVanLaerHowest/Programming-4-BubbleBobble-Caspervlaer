#include <cmath>
#include "WalkState.h"
#include "GameObject.h"
#include "../Components/AnimationComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/PlayerFacingComponent.h"
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
			auto* facing = m_pOwner->GetComponent<PlayerFacingComponent>();
			if (facing)
			{
				facing->SetFacingFromHorizontalMovement(velocity.x);
				animation->SetFlipHorizontal(facing->IsFacingRight());
			}
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
		auto* facing = m_pOwner->GetComponent<PlayerFacingComponent>();
		if (facing)
		{
			facing->SetFacingFromHorizontalMovement(velocity.x);
		}

		if (auto* animation = m_pOwner->GetComponent<AnimationComponent>())
		{
			animation->SetFlipHorizontal(facing && facing->IsFacingRight());
		}

		return nullptr;
	}

	return std::make_unique<IdleState>(m_pOwner);
}
