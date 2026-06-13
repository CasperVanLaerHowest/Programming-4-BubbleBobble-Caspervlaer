#pragma once

#include "Component.h"

class MaitaBallComponent final : public dae::Component
{
public:
	MaitaBallComponent(dae::GameObject* owner, int ownerPlayerIndex = -1);

	void Update(float deltaTime) override;
	void Break();
	bool IsBreaking() const { return m_IsBreaking; }
	int GetOwnerPlayerIndex() const { return m_OwnerPlayerIndex; }

private:
	int m_OwnerPlayerIndex{ -1 };
	float m_Lifetime{ 4.f };
	float m_BreakDuration{ 0.35f };
	float m_ElapsedTime{};
	bool m_IsBreaking{};
};
