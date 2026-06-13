#include "LevelFlowComponent.h"

#include "BubbleStateComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include "PlayerSlotComponent.h"
#include "SceneManager.h"
#include "../Events/GameEventQueue.h"

LevelFlowComponent::LevelFlowComponent(dae::GameObject* owner, GameMode gameMode, int levelIndex)
	: Component(owner)
	, m_GameMode{ gameMode }
	, m_LevelIndex{ levelIndex }
{
}

void LevelFlowComponent::Update(float deltaTime)
{
	if (m_TransitionRequested)
	{
		m_TransitionDelay -= deltaTime;
		if (m_TransitionDelay <= 0.f)
		{
			QueueTransition(m_PendingLevelWon);
		}
		return;
	}

	bool hasEnemy{};
	int playerCount{};
	int livingPlayers{};
	bool playerOneAlive{ true };
	bool playerTwoAlive{ true };
	bool hasPlayerOne{};
	bool hasPlayerTwo{};

	for (auto* collider : CollisionComponent::GetColliders())
	{
		if (collider == nullptr || collider->GetOwner()->IsDestroyed())
		{
			continue;
		}

		if (collider->GetCollisionType() == CollisionType::Enemy)
		{
			hasEnemy = true;
		}

		if (auto* bubble = collider->GetOwner()->GetComponent<BubbleStateComponent>())
		{
			if (bubble->IsTrapped())
			{
				hasEnemy = true;
			}
		}

		if (collider->GetCollisionType() == CollisionType::Player)
		{
			++playerCount;
			const auto* health = collider->GetOwner()->GetComponent<HealthComponent>();
			const bool isAlive = health == nullptr || health->GetHealth() > 0;
			if (isAlive)
			{
				++livingPlayers;
			}

			const auto* playerSlot = collider->GetOwner()->GetComponent<PlayerSlotComponent>();
			if (playerSlot != nullptr && playerSlot->GetPlayerIndex() == 0)
			{
				hasPlayerOne = true;
				playerOneAlive = isAlive;
			}
			if (playerSlot != nullptr && playerSlot->GetPlayerIndex() == 1)
			{
				hasPlayerTwo = true;
				playerTwoAlive = isAlive;
			}
		}
	}

	if (m_GameMode == GameMode::Versus)
	{
		if (hasPlayerOne && !playerOneAlive)
		{
			QueueTransition(false);
			return;
		}
		if (hasPlayerTwo && !playerTwoAlive)
		{
			QueueTransition(true);
			return;
		}

		return;
	}

	if (playerCount > 0 && livingPlayers == 0)
	{
		QueueTransition(false);
		return;
	}

	if (m_GameMode != GameMode::Versus && !hasEnemy)
	{
		StartDelayedWinTransition();
	}
}

void LevelFlowComponent::StartDelayedWinTransition()
{
	if (m_TransitionRequested)
		return;

	m_TransitionRequested = true;
	m_PendingLevelWon = true;
	m_TransitionDelay = 5.f;
}

void LevelFlowComponent::SkipLevel()
{
	if (m_TransitionRequested)
		return;

	m_TransitionRequested = true;
	m_PendingLevelWon = true;
	m_TransitionDelay = 0.f;
}

void LevelFlowComponent::QueueTransition(bool levelWon)
{
	if (m_TransitionQueued)
		return;

	m_TransitionRequested = true;
	m_TransitionQueued = true;
	const auto gameMode = m_GameMode;
	const auto levelIndex = m_LevelIndex;
	dae::SceneManager::GetInstance().QueuePostUpdateAction([gameMode, levelIndex, levelWon]()
		{
			if (gameMode == GameMode::Versus)
			{
				GameEventQueue::GetInstance().QueueEvent({
					GameEventType::VersusFinished,
					gameMode,
					levelIndex,
					levelWon
					});
				return;
			}

			GameEventQueue::GetInstance().QueueEvent({
				levelWon ? GameEventType::LevelCompleted : GameEventType::GameOver,
				gameMode,
				levelIndex,
				levelWon
				});
		});
}
