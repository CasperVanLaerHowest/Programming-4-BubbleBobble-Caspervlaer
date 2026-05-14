#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "BubbleStateComponent.h"
#include "../HelperFunctions/CollisionRules.h"
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

		if (auto bubble = GetOwner()->GetComponent<BubbleStateComponent>())
			bubble->PrepareMovement();

		SetCorrectVelocity(deltaTime);

		CollisionCheck(deltaTime, pos, collider);

		transform->SetLocalPosition(pos.x + (m_Velocity.x * deltaTime),
									pos.y + (m_Velocity.y * deltaTime),
									pos.z);
	}
}

bool PhysicsComponent::IsGrounded() const
{
	auto transform = GetOwner()->GetComponent<dae::TransformComponent>();
	auto collider = GetOwner()->GetComponent<CollisionComponent>();

	if (!transform || !collider)
		return false;

	auto pos = transform->GetLocalPosition();
	glm::vec2 groundCheckPos = { pos.x, pos.y + 1.f };

	for (auto otherCollider : CollisionComponent::GetColliders())
	{
		if (otherCollider == collider)
			continue;

		if (CollisionRules::IsGround(collider, otherCollider, pos, groundCheckPos))
			return true;
	}

	return false;
}

void PhysicsComponent::SetCorrectVelocity(float deltaTime)
{
	const bool isGrounded = IsGrounded();

	if (isGrounded && m_Velocity.y >= 0.f)
		m_Velocity.y = 0.f;
	else
		m_Velocity.y += m_Gravity * deltaTime;

	if (m_Velocity.y > m_MaxVerticalSpeed)
		m_Velocity.y = m_MaxVerticalSpeed;
	else if (m_Velocity.y < -m_MaxVerticalSpeed)
		m_Velocity.y = -m_MaxVerticalSpeed;

	if (m_Velocity.x > m_MaxHorizontalSpeed)
		m_Velocity.x = m_MaxHorizontalSpeed;
	else if (m_Velocity.x < -m_MaxHorizontalSpeed)
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
}

void PhysicsComponent::CollisionCheck(float deltaTime,  glm::vec3 pos, CollisionComponent* collider)
{
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

			if (CollisionRules::ShouldBlockX(collider, otherCollider, { pos.x, pos.y }, predictedPosX, m_Velocity.x))
				stopX = true;

			if (CollisionRules::ShouldBlockY(collider, otherCollider, { pos.x, pos.y }, predictedPosY, m_Velocity.y))
				stopY = true;

			HandleBubbleInteraction(collider, otherCollider, { pos.x, pos.y }, predictedPosX, predictedPosY);
		}
	}

	if (stopX) m_Velocity.x = 0;
	if (stopY) m_Velocity.y = 0;
}

void PhysicsComponent::HandleBubbleInteraction(
	CollisionComponent* collider,
	CollisionComponent* otherCollider,
	const glm::vec2& currentPosition,
	const glm::vec2& predictedPosX,
	const glm::vec2& predictedPosY)
{
	if (CollisionRules::ShouldBounceOnBubble(collider, otherCollider, currentPosition, predictedPosY, m_Velocity.y))
	{
		if (auto bubble = otherCollider->GetOwner()->GetComponent<BubbleStateComponent>())
			m_Velocity.y = -bubble->GetBounceVelocity();
	}

	if (!CollisionRules::ShouldPushBubble(collider, otherCollider, predictedPosX, m_Velocity.x))
		return;

	auto bubble = otherCollider->GetOwner()->GetComponent<BubbleStateComponent>();
	if (!bubble || !bubble->IsFloatingUp())
		return;

	const float pushDirection = m_Velocity.x > 0.f ? 1.f : -1.f;
	bubble->PushSideways(pushDirection);
}
