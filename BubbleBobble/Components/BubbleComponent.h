#pragma once
#include "FacingComponent.h"
#include "Component.h"

class BubbleComponent final : public dae::Component
{
	public:
	BubbleComponent(dae::GameObject* owner);
	virtual ~BubbleComponent() = default;
	void FixedUpdate(float) override {};
	void Update(float) override;
	void Render() const override {};

	void PrepareMovement();
	void PushSideways(float direction);
	float GetBounceVelocity() const { return m_BounceVelocity; }
	bool IsFloatingUp() const { return m_IsFloatingUp; }

private:
	void StartMovingForward() const;
	void StartFloatingUp() const;

	float m_Speed{ 250.f };
	float m_FloatSpeed{ 75.f };
	float m_BounceVelocity{ 350.f };
	float m_PushVelocity{ 120.f };
	float m_InitialMoveTime{ 0.5f };
	float m_Lifetime{ 10.f };
	float m_ElapsedTime{ 0.f };
	bool m_WasPushedThisFrame{ false };
	bool m_IsFloatingUp{ false };
	bool m_IsExpired{ false };
};
