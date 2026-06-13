#include "MaitaStateComponent.h"

#include "AnimationComponent.h"
#include "CollisionComponent.h"
#include "FacingComponent.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "PlayerStateComponent.h"
#include "TransformComponent.h"
#include "../HelperFunctions/GameObjectFactory.h"
#include "Service/AudioLocator.h"

#include <cmath>
#include <limits>

MaitaStateComponent::MaitaStateComponent(dae::GameObject* owner, dae::Scene& scene)
	: Component(owner)
	, m_Scene{ &scene }
{
}

void MaitaStateComponent::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;
	m_TargetUpdateTimer -= deltaTime;
	m_JumpTimer -= deltaTime;
	if (m_JumpTimer <= 0.f)
		m_IsWaitingAfterUpJump = false;

	UpdateTargetDirection();
	TryJumpAtPlayer();
	TryUseNavigationPoint();
	UpdateAttack(deltaTime);
	UpdateMovement();
	UpdateAnimation();
}

bool MaitaStateComponent::TryGetClosestPlayerPosition(glm::vec2& playerPosition) const
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

void MaitaStateComponent::UpdateTargetDirection()
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
		return;

	if (std::abs(playerPosition.x - position.x) > 8.f)
	{
		m_Direction = playerPosition.x < position.x ? -1.f : 1.f;
	}
}

bool MaitaStateComponent::HasPlayerAtAttackHeight() const
{
	glm::vec2 playerPosition{};
	if (!TryGetClosestPlayerPosition(playerPosition))
		return false;

	const auto position = GetPosition();
	return std::abs(playerPosition.y - position.y) <= m_AttackHeightMargin;
}

void MaitaStateComponent::TryJumpAtPlayer()
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

void MaitaStateComponent::TryUseNavigationPoint()
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

void MaitaStateComponent::UpdateMovement()
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

	if (m_IsAttacking)
	{
		velocity.x = 0.f;
	}
	else if (!isGrounded && !m_IsGapJumping)
	{
		velocity.x = 0.f;
	}
	else
	{
		const float speed = m_ElapsedTime >= m_AngryTime ? m_AngrySpeed : m_NormalSpeed;
		velocity.x = m_Direction * (m_IsGapJumping ? m_GapJumpHorizontalSpeed : speed);
	}
	physics->SetVelocity(velocity);
}

void MaitaStateComponent::UpdateAttack(float deltaTime)
{
	if (m_IsAttacking)
	{
		m_AttackElapsed += deltaTime;
		if (!m_HasShotBall && m_AttackElapsed >= m_AttackDuration * 0.45f)
		{
			ShootBall();
			m_HasShotBall = true;
		}

		if (m_AttackElapsed >= m_AttackDuration)
		{
			m_IsAttacking = false;
			m_AttackElapsed = 0.f;
		}
		return;
	}

	m_AttackTimer -= deltaTime;
	if (m_AttackTimer > 0.f)
		return;

	if (!HasPlayerAtAttackHeight())
		return;

	m_IsAttacking = true;
	m_HasShotBall = false;
	m_AttackElapsed = 0.f;
	m_AttackTimer = m_AttackInterval;
}

void MaitaStateComponent::UpdateAnimation() const
{
	if (auto* facing = GetOwner()->GetComponent<FacingComponent>())
	{
		facing->SetFacingFromHorizontalMovement(m_Direction);
	}

	auto* animation = GetOwner()->GetComponent<AnimationComponent>();
	if (animation == nullptr)
		return;

	const bool isAngry = m_ElapsedTime >= m_AngryTime;
	animation->PlayAnimation(m_IsAttacking
		? (isAngry ? "AngryAttack" : "Attack")
		: (isAngry ? "Angry" : "Move"));

	const auto* facing = GetOwner()->GetComponent<FacingComponent>();
	animation->SetFlipHorizontal(facing && facing->IsFacingRight());
}

void MaitaStateComponent::ShootBall()
{
	if (m_Scene == nullptr)
		return;

	const auto position = GetPosition();
	SpawnMaitaBall(*m_Scene, { position.x + m_Direction * 24.f, position.y - 10.f }, m_Direction);
	AudioLocator::GetAudio().PlaySound("Data/Sound/shoot.wav");
}

bool MaitaStateComponent::HasWallAhead() const
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

bool MaitaStateComponent::HasGroundAhead() const
{
	return HasGroundInDirection(m_Direction);
}

bool MaitaStateComponent::HasGroundInDirection(float direction) const
{
	return HasGroundAtHorizontalOffset(direction * m_EdgeLookAhead);
}

bool MaitaStateComponent::HasGroundAtHorizontalOffset(float offset) const
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

bool MaitaStateComponent::HasReachablePlatformAbove() const
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

bool MaitaStateComponent::HasPlatformAcrossGap(float direction) const
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

glm::vec2 MaitaStateComponent::GetPosition() const
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transform == nullptr)
		return {};

	const auto& position = transform->GetWorldPosition();
	return { position.x, position.y };
}
