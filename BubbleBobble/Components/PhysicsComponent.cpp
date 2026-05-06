#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include <cmath>

PhysicsComponent::PhysicsComponent(dae::GameObject* owner)
	: Component(owner)
{
}

void PhysicsComponent::Update(float deltaTime)
{
	auto transform = GetOwner()->GetComponent<dae::TransformComponent>();
	auto collider = GetOwner()->GetComponent<CollisionComponent>();

	if (transform)
	{
		auto pos = transform->GetLocalPosition();

		bool isGrounded = false;
		if (collider)
		{
			glm::vec2 groundCheckPos = { pos.x, pos.y + 1.f };
			for (auto otherCollider : CollisionComponent::GetColliders())
			{
				if (otherCollider == collider)
					continue;

				if (collider->CheckCollision(groundCheckPos, otherCollider->GetOwner()))
				{
					isGrounded = true;
					break;
				}
			}
		}

		if (isGrounded && m_Velocity.y >= 0.f)
			m_Velocity.y = 0.f;
		else
			m_Velocity.y += m_Gravity * deltaTime;

		if (m_Velocity.y > m_MaxVerticalSpeed)
			m_Velocity.y = m_MaxVerticalSpeed;
		else if (m_Velocity.y < -m_MaxVerticalSpeed)
			m_Velocity.y = -m_MaxVerticalSpeed;

		if(m_Velocity.x > m_MaxHorizontalSpeed)
			m_Velocity.x = m_MaxHorizontalSpeed;
		else if(m_Velocity.x < -m_MaxHorizontalSpeed)
			m_Velocity.x = -m_MaxHorizontalSpeed;

		const bool isNearGround = std::abs(m_Velocity.y) <= m_GroundVelocityEpsilon;
		const float friction = isNearGround ? m_GroundFriction : m_AirFriction;
		const float frictionDelta = friction * deltaTime;

		if (isNearGround && std::abs(m_Velocity.x) <= m_GroundStopVelocity)
		{
			m_Velocity.x = 0.f;
		}
		else if (m_Velocity.x > 0.f)
		{
			m_Velocity.x -= frictionDelta;
			if (m_Velocity.x < 0.f)
				m_Velocity.x = 0.f;
		}
		else if (m_Velocity.x < 0.f)
		{
			m_Velocity.x += frictionDelta;
			if (m_Velocity.x > 0.f)
				m_Velocity.x = 0.f;
		}

		bool stopX = false;
		bool stopY = false;

		if (collider)
		{
			glm::vec2 predictedPosX = { pos.x + (m_Velocity.x * deltaTime), pos.y };
			glm::vec2 predictedPosY = { pos.x, pos.y + (m_Velocity.y * deltaTime) };

			for (auto otherCollider : CollisionComponent::GetColliders())
			{
				if (otherCollider == collider)
					continue; 

				if (collider->CheckCollision(predictedPosX, otherCollider->GetOwner()))
					stopX = true;

				if (collider->CheckCollision(predictedPosY, otherCollider->GetOwner()))
					stopY = true;
			}
		}

		if (stopX) m_Velocity.x = 0;
		if (stopY) m_Velocity.y = 0;

		transform->SetLocalPosition(pos.x + (m_Velocity.x * deltaTime),
									pos.y + (m_Velocity.y * deltaTime),
									pos.z);
	}
}