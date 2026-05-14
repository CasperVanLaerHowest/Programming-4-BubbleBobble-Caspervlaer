#pragma once
#include <glm/ext/vector_float2.hpp>

class CollisionComponent;

class CollisionRules final
{
public:
	static bool ShouldBlockX(
		const CollisionComponent* collider,
		const CollisionComponent* otherCollider,
		const glm::vec2& currentPosition,
		const glm::vec2& predictedPosition,
		float horizontalVelocity);

	static bool ShouldBlockY(
		const CollisionComponent* collider,
		const CollisionComponent* otherCollider,
		const glm::vec2& currentPosition,
		const glm::vec2& predictedPosition,
		float verticalVelocity);

	static bool IsGround(
		const CollisionComponent* collider,
		const CollisionComponent* otherCollider,
		const glm::vec2& currentPosition,
		const glm::vec2& groundCheckPosition);

private:
	static bool IsMovingOntoPlatform(
		const CollisionComponent* collider,
		const CollisionComponent* platform,
		const glm::vec2& currentPosition,
		float verticalVelocity);

	static bool IsOverlappingAnyPlatform(
		const CollisionComponent* collider,
		const glm::vec2& currentPosition);
};
