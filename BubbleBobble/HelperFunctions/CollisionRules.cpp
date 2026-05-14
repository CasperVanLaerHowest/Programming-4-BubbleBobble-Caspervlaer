#include "CollisionRules.h"
#include "../Components/CollisionComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

namespace
{
	glm::vec2 GetColliderPosition(const CollisionComponent* collider)
	{
		auto transform = collider->GetOwner()->GetComponent<dae::TransformComponent>();
		const auto offset = collider->GetOffset();

		return {
			transform->GetWorldPosition().x + offset.x,
			transform->GetWorldPosition().y + offset.y
		};
	}
}

bool CollisionRules::ShouldBlockX(
	const CollisionComponent* collider,
	const CollisionComponent* otherCollider,
	const glm::vec2& currentPosition,
	const glm::vec2& predictedPosition,
	float horizontalVelocity)
{
	const auto otherType = otherCollider->GetCollisionType();

	if (otherType == CollisionType::Solid)
		return collider->CheckCollision(predictedPosition, otherCollider->GetOwner());

	if (otherType != CollisionType::Platform ||
		collider->GetCollisionType() != CollisionType::Bubble ||
		horizontalVelocity == 0.f ||
		IsOverlappingAnyPlatform(collider, currentPosition))
	{
		return false;
	}

	return !collider->CheckCollision(currentPosition, otherCollider->GetOwner()) &&
		collider->CheckCollision(predictedPosition, otherCollider->GetOwner());
}

bool CollisionRules::ShouldBlockY(
	const CollisionComponent* collider,
	const CollisionComponent* otherCollider,
	const glm::vec2& currentPosition,
	const glm::vec2& predictedPosition,
	float verticalVelocity)
{
	const auto otherType = otherCollider->GetCollisionType();

	if (otherType == CollisionType::Solid)
		return collider->CheckCollision(predictedPosition, otherCollider->GetOwner());

	if (otherType != CollisionType::Platform ||
		collider->GetCollisionType() == CollisionType::Bubble)
	{
		return false;
	}

	return collider->CheckCollision(predictedPosition, otherCollider->GetOwner()) &&
		IsMovingOntoPlatform(collider, otherCollider, currentPosition, verticalVelocity);
}

bool CollisionRules::IsGround(
	const CollisionComponent* collider,
	const CollisionComponent* otherCollider,
	const glm::vec2& currentPosition,
	const glm::vec2& groundCheckPosition)
{
	if (collider->GetCollisionType() == CollisionType::Bubble)
		return false;

	const auto otherType = otherCollider->GetCollisionType();

	if (otherType == CollisionType::Solid)
		return collider->CheckCollision(groundCheckPosition, otherCollider->GetOwner());

	if (otherType != CollisionType::Platform)
		return false;

	return collider->CheckCollision(groundCheckPosition, otherCollider->GetOwner()) &&
		IsMovingOntoPlatform(collider, otherCollider, currentPosition, 1.f);
}

bool CollisionRules::ShouldBounceOnBubble(
	const CollisionComponent* collider,
	const CollisionComponent* otherCollider,
	const glm::vec2& currentPosition,
	const glm::vec2& predictedPosition,
	float verticalVelocity)
{
	if (collider->GetCollisionType() != CollisionType::Player ||
		otherCollider->GetCollisionType() != CollisionType::Bubble)
	{
		return false;
	}

	return collider->CheckCollision(predictedPosition, otherCollider->GetOwner()) &&
		IsMovingOntoCollider(collider, otherCollider, currentPosition, verticalVelocity);
}

bool CollisionRules::ShouldPushBubble(
	const CollisionComponent* collider,
	const CollisionComponent* otherCollider,
	const glm::vec2& predictedPosition,
	float horizontalVelocity)
{
	if (horizontalVelocity == 0.f ||
		collider->GetCollisionType() != CollisionType::Player ||
		otherCollider->GetCollisionType() != CollisionType::Bubble)
	{
		return false;
	}

	return collider->CheckCollision(predictedPosition, otherCollider->GetOwner());
}

bool CollisionRules::IsMovingOntoPlatform(
	const CollisionComponent* collider,
	const CollisionComponent* platform,
	const glm::vec2& currentPosition,
	float verticalVelocity)
{
	if (verticalVelocity <= 0.f)
		return false;

	const auto colliderBottom = currentPosition.y + collider->GetOffset().y + collider->GetSize().y;
	const auto platformTop = GetColliderPosition(platform).y;

	return colliderBottom <= platformTop + 1.f;
}

bool CollisionRules::IsMovingOntoCollider(
	const CollisionComponent* collider,
	const CollisionComponent* otherCollider,
	const glm::vec2& currentPosition,
	float verticalVelocity)
{
	if (verticalVelocity <= 0.f)
		return false;

	const auto colliderBottom = currentPosition.y + collider->GetOffset().y + collider->GetSize().y;
	const auto otherTop = GetColliderPosition(otherCollider).y;

	return colliderBottom <= otherTop + 1.f;
}

bool CollisionRules::IsOverlappingAnyPlatform(
	const CollisionComponent* collider,
	const glm::vec2& currentPosition)
{
	for (auto otherCollider : CollisionComponent::GetColliders())
	{
		if (otherCollider == collider ||
			otherCollider->GetCollisionType() != CollisionType::Platform)
		{
			continue;
		}

		if (collider->CheckCollision(currentPosition, otherCollider->GetOwner()))
			return true;
	}

	return false;
}
