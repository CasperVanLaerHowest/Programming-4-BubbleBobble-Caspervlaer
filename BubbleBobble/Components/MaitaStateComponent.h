#pragma once

#include "Component.h"
#include <glm/ext/vector_float2.hpp>

namespace dae { class Scene; }

class MaitaStateComponent final : public dae::Component
{
public:
	MaitaStateComponent(dae::GameObject* owner, dae::Scene& scene);

	void Update(float deltaTime) override;

private:
	bool TryGetClosestPlayerPosition(glm::vec2& playerPosition) const;
	void UpdateTargetDirection();
	bool HasPlayerAtAttackHeight() const;
	void TryJumpAtPlayer();
	void TryUseNavigationPoint();
	void UpdateMovement();
	void UpdateAttack(float deltaTime);
	void UpdateAnimation() const;
	void ShootBall();
	bool HasWallAhead() const;
	bool HasGroundAhead() const;
	bool HasGroundInDirection(float direction) const;
	bool HasGroundAtHorizontalOffset(float offset) const;
	bool HasReachablePlatformAbove() const;
	bool HasPlatformAcrossGap(float direction) const;
	glm::vec2 GetPosition() const;

	dae::Scene* m_Scene{};
	float m_Direction{ -1.f };
	float m_ElapsedTime{};
	float m_TargetUpdateTimer{};
	float m_JumpTimer{ 1.f };
	float m_AttackTimer{ 1.5f };
	float m_AttackElapsed{};
	bool m_IsAttacking{};
	bool m_HasShotBall{};
	bool m_IsGapJumping{};
	bool m_IsWaitingAfterUpJump{};

	float m_AngryTime{ 18.f };
	float m_AttackInterval{ 3.2f };
	float m_AttackDuration{ 0.55f };
	float m_AttackHeightMargin{ 18.f };
	float m_NormalSpeed{ 45.f };
	float m_AngrySpeed{ 75.f };
	float m_JumpStrength{ 330.f };
	float m_GapJumpStrength{ 250.f };
	float m_GapJumpHorizontalSpeed{ 120.f };
	float m_TargetUpdateInterval{ 1.f };
	float m_JumpInterval{ 2.5f };
	float m_UpJumpInterval{ 1.5f };
	float m_EdgeLookAhead{ 18.f };
	float m_WallLookAhead{ 5.f };
	float m_GroundProbeDistance{ 8.f };
	float m_UpJumpSearchHeight{ 140.f };
	float m_UpJumpHorizontalMargin{ 6.f };
	float m_GapJumpSearchStep{ 6.f };
	int m_GapJumpSearchSteps{ 30 };
};
