#pragma once
#include "Component.h"
#include "../States/BaseState.h"
#include <glm/ext/vector_float2.hpp>
#include <memory>
#include <string>

class CollisionComponent;

class EnemyStateComponent final : public dae::Component
{
public:
	EnemyStateComponent(dae::GameObject* owner);
	virtual ~EnemyStateComponent() = default;

	void Update(float deltaTime) override;
	void ChangeState(std::unique_ptr<BaseState> newState);

	void UpdateTargetDirection(float deltaTime);
	void Move(float speed);
	void TryJumpAtPlayer();
	void TryUseNavigationPoint();
	void UpdateFacingAndAnimation(const std::string& animationName) const;
	bool HasWallAhead() const;
	bool HasGroundAhead() const;
	bool HasGroundInDirection(float direction) const;
	bool TryGetClosestPlayerPosition(glm::vec2& playerPosition) const;
	glm::vec2 GetPosition() const;
	float GetElapsedTime() const { return m_ElapsedTime; }
	float GetAngryTime() const { return m_AngryTime; }
	float GetNormalSpeed() const { return m_NormalSpeed; }
	float GetAngrySpeed() const { return m_AngrySpeed; }

private:
	bool TryFindDropDirection(float playerX, float& direction) const;
	bool HasReachablePlatformAbove() const;
	bool HasPlatformAcrossGap(float direction) const;
	bool HasGroundAtHorizontalOffset(float offset) const;

	std::unique_ptr<BaseState> m_pCurrentState;

	float m_Direction{ -1.f };
	float m_TargetUpdateTimer{};
	float m_JumpTimer{ 1.f };
	float m_ElapsedTime{};
	bool m_IsGapJumping{};
	bool m_IsWaitingAfterUpJump{};

	float m_NormalSpeed{ 65.f };
	float m_AngrySpeed{ 105.f };
	float m_JumpStrength{ 330.f };
	float m_GapJumpStrength{ 250.f };
	float m_GapJumpHorizontalSpeed{ 120.f };
	float m_AngryTime{ 18.f };
	float m_TargetUpdateInterval{ 1.f };
	float m_JumpInterval{ 2.5f };
	float m_UpJumpInterval{ 1.5f };
	float m_EdgeLookAhead{ 18.f };
	float m_WallLookAhead{ 5.f };
	float m_GroundProbeDistance{ 8.f };
	float m_UpJumpSearchHeight{ 140.f };
	float m_UpJumpHorizontalMargin{ 6.f };
	float m_DropSearchStep{ 12.f };
	int m_DropSearchSteps{ 30 };
	float m_GapJumpSearchStep{ 6.f };
	int m_GapJumpSearchSteps{ 30 };
};
