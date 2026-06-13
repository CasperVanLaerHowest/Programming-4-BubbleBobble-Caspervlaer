#include "MaitaBallComponent.h"

#include "AnimationComponent.h"
#include "GameObject.h"
#include "PhysicsComponent.h"

MaitaBallComponent::MaitaBallComponent(dae::GameObject* owner, int ownerPlayerIndex)
	: Component(owner)
	, m_OwnerPlayerIndex{ ownerPlayerIndex }
{
}

void MaitaBallComponent::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;

	if (!m_IsBreaking && m_ElapsedTime >= m_Lifetime)
	{
		Break();
		return;
	}

	if (m_IsBreaking && m_ElapsedTime >= m_BreakDuration)
	{
		GetOwner()->Destroy();
	}
}

void MaitaBallComponent::Break()
{
	if (m_IsBreaking)
		return;

	m_IsBreaking = true;
	m_ElapsedTime = 0.f;

	if (auto* physics = GetOwner()->GetComponent<PhysicsComponent>())
	{
		physics->SetVelocity({ 0.f, 0.f });
		physics->SetGravity(0.f);
	}

	if (auto* animation = GetOwner()->GetComponent<AnimationComponent>())
	{
		animation->PlayAnimation("Break");
	}
}
