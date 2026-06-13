#include "PlayerDeathState.h"
#include "IdleState.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/PhysicsComponent.h"

PlayerDeathState::PlayerDeathState(dae::GameObject* pOwner, const glm::vec2& respawnPosition)
	: BaseState(pOwner)
	, m_RespawnPosition{ respawnPosition }
{
}

void PlayerDeathState::Enter()
{
	if (auto* physics = m_pOwner->GetComponent<PhysicsComponent>())
	{
		physics->SetVelocity({ 0.f, 0.f });
		physics->SetGravity(0.f);
	}

	if (auto* animation = m_pOwner->GetComponent<AnimationComponent>())
	{
		animation->PlayAnimation("Die");
	}
}

std::unique_ptr<BaseState> PlayerDeathState::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;

	if (m_ElapsedTime < m_DeathDuration)
		return nullptr;

	const auto* health = m_pOwner->GetComponent<HealthComponent>();
	if (health == nullptr || health->GetHealth() <= 0)
	{
		if (auto* physics = m_pOwner->GetComponent<PhysicsComponent>())
		{
			physics->SetVelocity({ 0.f, 0.f });
		}
		return nullptr;
	}

	if (auto* transform = m_pOwner->GetComponent<dae::TransformComponent>())
	{
		transform->SetLocalPosition(m_RespawnPosition.x, m_RespawnPosition.y, 0.f);
	}

	if (auto* physics = m_pOwner->GetComponent<PhysicsComponent>())
	{
		physics->SetVelocity({ 0.f, 0.f });
		physics->ResetGravity();
	}

	return std::make_unique<IdleState>(m_pOwner);
}
