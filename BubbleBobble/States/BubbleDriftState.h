#pragma once
#include "BubbleBaseState.h"

class BubbleDriftState final : public BubbleBaseState
{
public:
	BubbleDriftState(dae::GameObject* pOwner) : BubbleBaseState(pOwner) {}
	virtual ~BubbleDriftState() = default;

	void Enter() override;
	std::unique_ptr<BaseState> Update(float deltaTime) override;
	void HandleInput() override {}
	void Exit() override {}

	void PrepareMovement() override;
	void PushSideways(float direction) override;
	bool IsFloatingUp() const override { return true; }

private:
	bool m_WasPushedThisFrame{ false };
};
