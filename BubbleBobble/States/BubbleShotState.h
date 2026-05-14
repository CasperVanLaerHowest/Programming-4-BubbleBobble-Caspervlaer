#pragma once
#include "BubbleBaseState.h"

class BubbleShotState final : public BubbleBaseState
{
public:
	BubbleShotState(dae::GameObject* pOwner) : BubbleBaseState(pOwner) {}
	virtual ~BubbleShotState() = default;

	void Enter() override;
	std::unique_ptr<BaseState> Update(float deltaTime) override;
	void HandleInput() override {}
	void Exit() override {}
};
