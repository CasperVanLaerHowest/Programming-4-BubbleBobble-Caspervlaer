#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "BubbleStateComponent.h"
#include "EnemyDropComponent.h"
#include "MaitaBallComponent.h"
#include "MaitaPlayerComponent.h"
#include "PlayerStateComponent.h"
#include "PlayerSlotComponent.h"
#include "PoppedEnemyComponent.h"
#include "ScoreComponent.h"
#include "../HelperFunctions/CollisionRules.h"
#include "Service/AudioLocator.h"
#include "GameObject.h"
#include <cmath>

namespace
{
	constexpr float ScreenTop{ 0.f };
	constexpr float ScreenBottom{ 576.f };
}

PhysicsComponent::PhysicsComponent(dae::GameObject* owner)
	: Component(owner)
{
}

void PhysicsComponent::Update(float deltaTime)
{
	if (GetOwner()->IsDestroyed())
		return;

	if (auto* maitaPlayer = GetOwner()->GetComponent<MaitaPlayerComponent>(); maitaPlayer && maitaPlayer->IsTrapped())
		return;

	auto transform = GetOwner()->GetComponent<dae::TransformComponent>();
	auto collider = GetOwner()->GetComponent<CollisionComponent>();

	if (transform)
	{
		auto pos = transform->GetLocalPosition();

		if (auto bubble = GetOwner()->GetComponent<BubbleStateComponent>())
			bubble->PrepareMovement();

		SetCorrectVelocity(deltaTime);

		CollisionCheck(deltaTime, pos, collider);

		if (GetOwner()->IsDestroyed())
			return;

		glm::vec3 nextPosition{
			pos.x + (m_Velocity.x * deltaTime),
			pos.y + (m_Velocity.y * deltaTime),
			pos.z
		};
		ApplyVerticalScreenWrap(nextPosition, collider);

		transform->SetLocalPosition(nextPosition.x, nextPosition.y, nextPosition.z);
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
		if (otherCollider == collider || otherCollider->GetOwner()->IsDestroyed())
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
			if (otherCollider == collider || otherCollider->GetOwner()->IsDestroyed())
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

	if (stopX && collider != nullptr && collider->GetCollisionType() == CollisionType::MaitaBall)
	{
		if (auto* maitaBall = GetOwner()->GetComponent<MaitaBallComponent>())
		{
			maitaBall->Break();
		}
	}
}

void PhysicsComponent::HandleBubbleInteraction(
	CollisionComponent* collider,
	CollisionComponent* otherCollider,
	const glm::vec2& currentPosition,
	const glm::vec2& predictedPosX,
	const glm::vec2& predictedPosY)
{
	if (CollisionRules::ShouldTrapEnemy(collider, otherCollider, predictedPosX, predictedPosY))
	{
		auto bubbleOwner = collider->GetCollisionType() == CollisionType::Bubble
			? collider->GetOwner()
			: otherCollider->GetOwner();
		auto enemyOwner = collider->GetCollisionType() == CollisionType::Enemy
			? collider->GetOwner()
			: otherCollider->GetOwner();

		auto bubble = bubbleOwner->GetComponent<BubbleStateComponent>();
		if (bubble == nullptr || !bubble->CanTrapEnemy())
			return;

		if (const auto* enemyDrop = enemyOwner->GetComponent<EnemyDropComponent>())
		{
			bubble->TrapEnemy(enemyDrop->GetFruitTexture(), enemyDrop->GetScoreValue());
		}
		else
		{
			bubble->TrapEnemy();
		}

		enemyOwner->Destroy();
	}

	const bool bubbleHitsMaitaPlayer =
		collider->GetCollisionType() == CollisionType::Bubble &&
		otherCollider->GetCollisionType() == CollisionType::Player &&
		otherCollider->GetOwner()->GetComponent<MaitaPlayerComponent>() != nullptr;

	const bool maitaPlayerHitsBubble =
		collider->GetCollisionType() == CollisionType::Player &&
		collider->GetOwner()->GetComponent<MaitaPlayerComponent>() != nullptr &&
		otherCollider->GetCollisionType() == CollisionType::Bubble;

	if (bubbleHitsMaitaPlayer || maitaPlayerHitsBubble)
	{
		auto* bubbleOwner = bubbleHitsMaitaPlayer ? collider->GetOwner() : otherCollider->GetOwner();
		auto* maitaOwner = bubbleHitsMaitaPlayer ? otherCollider->GetOwner() : collider->GetOwner();
		auto* bubble = bubbleOwner->GetComponent<BubbleStateComponent>();
		if (bubble != nullptr && bubble->CanTrapEnemy() &&
			(collider->CheckCollision(predictedPosX, otherCollider->GetOwner()) ||
				collider->CheckCollision(predictedPosY, otherCollider->GetOwner())))
		{
			if (auto* maitaPlayer = maitaOwner->GetComponent<MaitaPlayerComponent>())
			{
				bubble->TrapMaitaPlayer(maitaPlayer);
			}
			return;
		}
	}

	if (CollisionRules::ShouldDamagePlayer(collider, otherCollider, predictedPosX, predictedPosY))
	{
		auto playerOwner = collider->GetCollisionType() == CollisionType::Player
			? collider->GetOwner()
			: otherCollider->GetOwner();
		auto attackerOwner = collider->GetCollisionType() == CollisionType::Player
			? otherCollider->GetOwner()
			: collider->GetOwner();

		if (const auto* maitaBall = attackerOwner->GetComponent<MaitaBallComponent>(); maitaBall && maitaBall->IsBreaking())
			return;

		if (const auto* maitaBall = attackerOwner->GetComponent<MaitaBallComponent>())
		{
			const auto* playerSlot = playerOwner->GetComponent<PlayerSlotComponent>();
			if (playerSlot != nullptr && playerSlot->GetPlayerIndex() == maitaBall->GetOwnerPlayerIndex())
				return;
		}

		if (auto* playerState = playerOwner->GetComponent<PlayerStateComponent>())
			playerState->TakeHit();

		if (auto* maitaBall = attackerOwner->GetComponent<MaitaBallComponent>())
			maitaBall->Break();
	}

	if (CollisionRules::ShouldCollectFruit(collider, otherCollider, predictedPosX, predictedPosY))
	{
		auto playerOwner = collider->GetCollisionType() == CollisionType::Player
			? collider->GetOwner()
			: otherCollider->GetOwner();
		auto fruitOwner = collider->GetCollisionType() == CollisionType::Fruit
			? collider->GetOwner()
			: otherCollider->GetOwner();

		const auto* poppedEnemy = fruitOwner->GetComponent<PoppedEnemyComponent>();
		const int scoreValue = poppedEnemy != nullptr
			? poppedEnemy->GetScoreValue()
			: 100;

		if (auto* score = playerOwner->GetComponent<ScoreComponent>())
			score->AddScore(scoreValue);

		AudioLocator::GetAudio().PlaySound("Data/Sound/fruitPickup.wav");
		fruitOwner->Destroy();
		return;
	}

	if (CollisionRules::ShouldBounceOnBubble(collider, otherCollider, currentPosition, predictedPosY, m_Velocity.y))
	{
		if (auto bubble = otherCollider->GetOwner()->GetComponent<BubbleStateComponent>())
		{
			m_Velocity.y = -bubble->GetBounceVelocity();

			if (bubble->PopTrappedIntoFruit())
				return;
		}
	}

	if (!CollisionRules::ShouldPushBubble(collider, otherCollider, predictedPosX, m_Velocity.x))
		return;

	auto bubble = otherCollider->GetOwner()->GetComponent<BubbleStateComponent>();
	if (!bubble)
		return;

	if (bubble->PopTrappedIntoFruit())
		return;

	if (!bubble->IsFloatingUp())
		return;

	const float pushDirection = m_Velocity.x > 0.f ? 1.f : -1.f;
	bubble->PushSideways(pushDirection);
}

void PhysicsComponent::ApplyVerticalScreenWrap(glm::vec3& position, const CollisionComponent* collider) const
{
	if (collider == nullptr)
		return;

	const auto offset = collider->GetOffset();
	const auto size = collider->GetSize();
	const float colliderTop = position.y + offset.y;
	const float colliderBottom = colliderTop + size.y;

	if (colliderTop > ScreenBottom)
	{
		position.y = ScreenTop - offset.y - size.y;
	}
	else if (colliderBottom < ScreenTop)
	{
		position.y = ScreenBottom - offset.y;
	}
}
