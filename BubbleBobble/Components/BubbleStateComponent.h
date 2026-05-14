#pragma once
#include "Component.h"
#include "../States/BaseState.h"
#include <memory>

class BubbleBaseState;

class BubbleStateComponent final : public dae::Component
{
public:
	BubbleStateComponent(dae::GameObject* owner);
	virtual ~BubbleStateComponent() = default;
	void FixedUpdate(float) override {};
	void Update(float deltaTime) override;
	void Render() const override {};

	void Start();
	void ChangeState(std::unique_ptr<BaseState> newState);
	void PrepareMovement();
	void PushSideways(float direction);
	void TrapEnemy();
	void Pop();

	float GetSpeed() const { return m_Speed; }
	float GetFloatSpeed() const { return m_FloatSpeed; }
	float GetBounceVelocity() const { return m_BounceVelocity; }
	float GetPushVelocity() const { return m_PushVelocity; }
	float GetInitialMoveTime() const { return m_InitialMoveTime; }
	float GetLifetime() const { return m_Lifetime; }
	float GetElapsedTime() const { return m_ElapsedTime; }
	bool IsFloatingUp() const;

private:
	BubbleBaseState* GetCurrentBubbleState() const;

	std::unique_ptr<BaseState> m_pCurrentState;

	float m_Speed{ 250.f };
	float m_FloatSpeed{ 75.f };
	float m_BounceVelocity{ 350.f };
	float m_PushVelocity{ 120.f };
	float m_InitialMoveTime{ 0.5f };
	float m_Lifetime{ 10.f };
	float m_ElapsedTime{ 0.f };
};
