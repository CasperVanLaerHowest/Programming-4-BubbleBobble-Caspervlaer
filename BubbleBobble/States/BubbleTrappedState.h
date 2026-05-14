#pragma once
#include "BubbleBaseState.h"

class BubbleTrappedState final : public BubbleBaseState
{
public:
	BubbleTrappedState(dae::GameObject* pOwner) : BubbleBaseState(pOwner) {}
	virtual ~BubbleTrappedState() = default;

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
