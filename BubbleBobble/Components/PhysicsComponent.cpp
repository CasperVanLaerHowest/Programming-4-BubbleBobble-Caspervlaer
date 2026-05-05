#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

PhysicsComponent::PhysicsComponent(dae::GameObject* owner)
	: Component(owner)
{
}

void PhysicsComponent::Update(float deltaTime)
{
	auto transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transform)
	{
		auto pos = transform->GetLocalPosition();
		m_Velocity.y += m_Gravity * deltaTime;

		if (m_Velocity.y > m_MaxVerticalSpeed)
			m_Velocity.y = m_MaxVerticalSpeed;
		else if (m_Velocity.y < -m_MaxVerticalSpeed)
			m_Velocity.y = -m_MaxVerticalSpeed;

		if(m_Velocity.x > m_MaxHorizontalSpeed)
			m_Velocity.x = m_MaxHorizontalSpeed;
		else if(m_Velocity.x < -m_MaxHorizontalSpeed)
			m_Velocity.x = -m_MaxHorizontalSpeed;

		transform->SetLocalPosition(pos.x + (m_Velocity.x * deltaTime),
									pos.y + (m_Velocity.y * deltaTime),
									pos.z);
	}
}