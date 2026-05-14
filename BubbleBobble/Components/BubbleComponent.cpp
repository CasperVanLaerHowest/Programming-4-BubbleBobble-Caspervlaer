#include "BubbleComponent.h"
#include "GameObject.h"
#include "PhysicsComponent.h"

BubbleComponent::BubbleComponent(dae::GameObject* owner)
	: Component(owner)
{
	auto physics = GetOwner()->GetComponent<PhysicsComponent>();
	if (physics)
	{
		physics->SetGravity(0.f);
		physics->SetMaxHorizontalSpeed(m_Speed);
		physics->SetMaxVerticalSpeed(m_Speed);
		physics->SetFriction(0.f, 0.f);
		StartMovingForward();
	}
}

void BubbleComponent::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;

	if (m_ElapsedTime >= m_Lifetime)
	{
		if (!m_IsExpired)
		{
			if (auto physics = GetOwner()->GetComponent<PhysicsComponent>())
				physics->SetVelocity({ 0.f, 0.f });

			m_IsExpired = true;
		}
		return;
	}

	if (!m_IsFloatingUp && m_ElapsedTime > m_InitialMoveTime)
	{
		StartFloatingUp();
		m_IsFloatingUp = true;
	}
}

void BubbleComponent::StartMovingForward() const
{
	auto physics = GetOwner()->GetComponent<PhysicsComponent>();
	if (!physics)
		return;

	auto facing = GetOwner()->GetComponent<FacingComponent>();
	const float direction = facing && !facing->IsFacingRight() ? -1.f : 1.f;
	physics->SetVelocity({ direction * m_Speed, 0.f });
}

void BubbleComponent::StartFloatingUp() const
{
	auto physics = GetOwner()->GetComponent<PhysicsComponent>();
	if (!physics)
		return;

	physics->SetVelocity({ 0.f, -m_Speed * 0.3f });
}
