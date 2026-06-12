#include "EnemyStateComponent.h"
#include "AnimationComponent.h"
#include "CollisionComponent.h"
#include "FacingComponent.h"
#include "PhysicsComponent.h"
#include "PlayerStateComponent.h"
#include "../States/EnemyWalkState.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <cmath>
#include <limits>

EnemyStateComponent::EnemyStateComponent(dae::GameObject* owner)
	: Component(owner)
{
	ChangeState(std::make_unique<EnemyWalkState>(owner));
}

void EnemyStateComponent::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;
	m_TargetUpdateTimer -= deltaTime;
	m_JumpTimer -= deltaTime;
	if (m_JumpTimer <= 0.f)
		m_IsWaitingAfterUpJump = false;

	if (m_pCurrentState)
	{
		auto newState = m_pCurrentState->Update(deltaTime);
		if (newState)
		{
			ChangeState(std::move(newState));
		}
	}
}

void EnemyStateComponent::ChangeState(std::unique_ptr<BaseState> newState)
{
	if (m_pCurrentState)
		m_pCurrentState->Exit();

	m_pCurrentState = std::move(newState);

	if (m_pCurrentState)
		m_pCurrentState->Enter();
}

void EnemyStateComponent::UpdateTargetDirection(float)
{
	auto* physics = GetOwner()->GetComponent<PhysicsComponent>();
	if (physics != nullptr && !physics->IsGrounded())
		return;

	if (m_IsWaitingAfterUpJump && m_JumpTimer > 0.f)
		return;

	if (m_TargetUpdateTimer > 0.f)
		return;

	m_TargetUpdateTimer = m_TargetUpdateInterval;

	glm::vec2 playerPosition{};
	if (!TryGetClosestPlayerPosition(playerPosition))
		return;

	const auto position = GetPosition();
	const bool playerIsBelow = playerPosition.y > position.y + 35.f;

	if (playerIsBelow)
	{
		float dropDirection{};
		if (TryFindDropDirection(playerPosition.x, dropDirection))
		{
			m_Direction = dropDirection;
			return;
		}
	}

	if (std::abs(playerPosition.x - position.x) > 8.f)
	{
		m_Direction = playerPosition.x < position.x ? -1.f : 1.f;
	}
}

void EnemyStateComponent::Move(float speed)
{
	auto* physics = GetOwner()->GetComponent<PhysicsComponent>();
	if (physics == nullptr)
		return;

	const bool isGrounded = physics->IsGrounded();
	if (isGrounded)
		m_IsGapJumping = false;

	auto velocity = physics->GetVelocity();
	if (isGrounded && m_IsWaitingAfterUpJump && m_JumpTimer > 0.f)
	{
		velocity.x = 0.f;
		physics->SetVelocity(velocity);
		return;
	}

	if (HasWallAhead())
	{
		m_Direction *= -1.f;
	}

	if (!isGrounded && !m_IsGapJumping)
	{
		velocity.x = 0.f;
	}
	else
	{
		velocity.x = m_Direction * (m_IsGapJumping ? m_GapJumpHorizontalSpeed : speed);
	}
	physics->SetVelocity(velocity);
}

void EnemyStateComponent::TryJumpAtPlayer()
{
	auto* physics = GetOwner()->GetComponent<PhysicsComponent>();
	if (physics == nullptr)
		return;

	auto velocity = physics->GetVelocity();
	glm::vec2 playerPosition{};
	const auto position = GetPosition();
	if (!TryGetClosestPlayerPosition(playerPosition))
		return;

	const bool playerIsAbove = playerPosition.y < position.y - 35.f;
	const bool blocked = HasWallAhead();
	if ((playerIsAbove || blocked) && HasReachablePlatformAbove() && physics->IsGrounded() && m_JumpTimer <= 0.f)
	{
		velocity.y = -m_JumpStrength;
		velocity.x = 0.f;
		m_IsGapJumping = false;
		m_JumpTimer = playerIsAbove ? m_UpJumpInterval : m_JumpInterval;
		m_IsWaitingAfterUpJump = playerIsAbove;
		physics->SetVelocity(velocity);
	}
}

void EnemyStateComponent::TryUseNavigationPoint()
{
	auto* physics = GetOwner()->GetComponent<PhysicsComponent>();
	if (physics == nullptr || !physics->IsGrounded() || m_JumpTimer > 0.f)
		return;

	if (HasGroundAhead())
		return;

	glm::vec2 playerPosition{};
	if (!TryGetClosestPlayerPosition(playerPosition))
		return;

	const auto position = GetPosition();
	const bool playerIsAbove = playerPosition.y < position.y - 35.f;
	const bool playerIsBelow = playerPosition.y > position.y + 35.f;
	if (playerIsBelow || (!playerIsAbove && !HasPlatformAcrossGap(m_Direction)))
		return;

	auto velocity = physics->GetVelocity();
	velocity.y = -m_GapJumpStrength;
	velocity.x = m_Direction * m_GapJumpHorizontalSpeed;
	physics->SetVelocity(velocity);
	m_IsGapJumping = true;
	m_IsWaitingAfterUpJump = false;
	m_JumpTimer = m_JumpInterval;
}

void EnemyStateComponent::UpdateFacingAndAnimation(const std::string& animationName) const
{
	auto* facing = GetOwner()->GetComponent<FacingComponent>();
	if (facing != nullptr)
	{
		facing->SetFacingFromHorizontalMovement(m_Direction);
	}

	auto* animation = GetOwner()->GetComponent<AnimationComponent>();
	if (animation == nullptr)
		return;

	animation->PlayAnimation(animationName);
	animation->SetFlipHorizontal(facing && facing->IsFacingRight());
}

bool EnemyStateComponent::HasWallAhead() const
{
	auto* collider = GetOwner()->GetComponent<CollisionComponent>();
	if (collider == nullptr)
		return false;

	const auto position = GetPosition();
	const glm::vec2 checkPosition{ position.x + m_Direction * m_WallLookAhead, position.y };

	for (auto* otherCollider : CollisionComponent::GetColliders())
	{
		if (otherCollider == collider || otherCollider->GetOwner()->IsDestroyed())
			continue;

		if (otherCollider->GetCollisionType() != CollisionType::Solid)
			continue;

		if (collider->CheckCollision(checkPosition, otherCollider->GetOwner()))
			return true;
	}

	return false;
}

bool EnemyStateComponent::HasGroundAhead() const
{
	return HasGroundInDirection(m_Direction);
}

bool EnemyStateComponent::HasGroundInDirection(float direction) const
{
	return HasGroundAtHorizontalOffset(direction * m_EdgeLookAhead);
}

bool EnemyStateComponent::HasGroundAtHorizontalOffset(float offset) const
{
	auto* collider = GetOwner()->GetComponent<CollisionComponent>();
	if (collider == nullptr)
		return true;

	const auto position = GetPosition();
	const glm::vec2 checkPosition{
		position.x + offset,
		position.y + m_GroundProbeDistance
	};

	for (auto* otherCollider : CollisionComponent::GetColliders())
	{
		if (otherCollider == collider || otherCollider->GetOwner()->IsDestroyed())
			continue;

		const auto type = otherCollider->GetCollisionType();
		if (type != CollisionType::Solid && type != CollisionType::Platform)
			continue;

		if (collider->CheckCollision(checkPosition, otherCollider->GetOwner()))
			return true;
	}

	return false;
}

bool EnemyStateComponent::TryFindDropDirection(float playerX, float& direction) const
{
	const auto position = GetPosition();
	float closestLeftDrop = (std::numeric_limits<float>::max)();
	float closestRightDrop = (std::numeric_limits<float>::max)();

	for (int step = 1; step <= m_DropSearchSteps; ++step)
	{
		const float distance = step * m_DropSearchStep;
		if (closestLeftDrop == (std::numeric_limits<float>::max)() && !HasGroundAtHorizontalOffset(-distance))
			closestLeftDrop = distance;

		if (closestRightDrop == (std::numeric_limits<float>::max)() && !HasGroundAtHorizontalOffset(distance))
			closestRightDrop = distance;
	}

	const bool foundLeft = closestLeftDrop != (std::numeric_limits<float>::max)();
	const bool foundRight = closestRightDrop != (std::numeric_limits<float>::max)();
	if (!foundLeft && !foundRight)
		return false;

	if (foundLeft && !foundRight)
	{
		direction = -1.f;
		return true;
	}

	if (!foundLeft && foundRight)
	{
		direction = 1.f;
		return true;
	}

	const float leftDropX = position.x - closestLeftDrop;
	const float rightDropX = position.x + closestRightDrop;
	const float leftDistanceToPlayer = std::abs(playerX - leftDropX);
	const float rightDistanceToPlayer = std::abs(playerX - rightDropX);
	direction = leftDistanceToPlayer < rightDistanceToPlayer ? -1.f : 1.f;
	return true;
}

bool EnemyStateComponent::HasReachablePlatformAbove() const
{
	auto* collider = GetOwner()->GetComponent<CollisionComponent>();
	if (collider == nullptr)
		return false;

	const auto position = GetPosition();
	const auto colliderSize = collider->GetSize();
	const float enemyCenterX = position.x + colliderSize.x * 0.5f;
	const float enemyTop = position.y;

	for (auto* otherCollider : CollisionComponent::GetColliders())
	{
		if (otherCollider == collider || otherCollider->GetOwner()->IsDestroyed())
			continue;

		const auto type = otherCollider->GetCollisionType();
		if (type != CollisionType::Solid && type != CollisionType::Platform)
			continue;

		auto* otherTransform = otherCollider->GetOwner()->GetComponent<dae::TransformComponent>();
		if (otherTransform == nullptr)
			continue;

		const auto& otherPosition = otherTransform->GetWorldPosition();
		const auto otherOffset = otherCollider->GetOffset();
		const auto otherSize = otherCollider->GetSize();
		const float platformTop = otherPosition.y + otherOffset.y;
		const float platformLeft = otherPosition.x + otherOffset.x;
		const float platformRight = platformLeft + otherSize.x;

		const float verticalDistance = enemyTop - platformTop;
		if (verticalDistance <= 0.f || verticalDistance > m_UpJumpSearchHeight)
			continue;

		if (enemyCenterX >= platformLeft - m_UpJumpHorizontalMargin &&
			enemyCenterX <= platformRight + m_UpJumpHorizontalMargin)
		{
			return true;
		}
	}

	return false;
}

bool EnemyStateComponent::HasPlatformAcrossGap(float direction) const
{
	if (HasGroundInDirection(direction))
		return false;

	bool hasSeenGap = false;
	for (int step = 2; step <= m_GapJumpSearchSteps; ++step)
	{
		const float distance = step * m_GapJumpSearchStep;
		const bool hasGround = HasGroundAtHorizontalOffset(direction * distance);
		if (!hasGround)
		{
			hasSeenGap = true;
			continue;
		}

		if (hasSeenGap)
			return true;
	}

	return false;
}

bool EnemyStateComponent::TryGetClosestPlayerPosition(glm::vec2& playerPosition) const
{
	const auto position = GetPosition();
	float closestDistanceSquared = (std::numeric_limits<float>::max)();
	bool foundPlayer = false;

	for (auto* collider : CollisionComponent::GetColliders())
	{
		if (collider->GetCollisionType() != CollisionType::Player || collider->GetOwner()->IsDestroyed())
			continue;

		auto* playerState = collider->GetOwner()->GetComponent<PlayerStateComponent>();
		if (playerState != nullptr && playerState->IsDying())
			continue;

		auto* transform = collider->GetOwner()->GetComponent<dae::TransformComponent>();
		if (transform == nullptr)
			continue;

		const auto& playerWorldPosition = transform->GetWorldPosition();
		const glm::vec2 candidatePosition{ playerWorldPosition.x, playerWorldPosition.y };
		const glm::vec2 difference = candidatePosition - position;
		const float distanceSquared = difference.x * difference.x + difference.y * difference.y;

		if (distanceSquared < closestDistanceSquared)
		{
			closestDistanceSquared = distanceSquared;
			playerPosition = candidatePosition;
			foundPlayer = true;
		}
	}

	return foundPlayer;
}

glm::vec2 EnemyStateComponent::GetPosition() const
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transform == nullptr)
		return {};

	const auto& position = transform->GetWorldPosition();
	return { position.x, position.y };
}
