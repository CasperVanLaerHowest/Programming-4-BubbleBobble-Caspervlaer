#pragma once
#include "BubbleBaseState.h"

class BubblePopState final : public BubbleBaseState
{
public:
	BubblePopState(dae::GameObject* pOwner) : BubbleBaseState(pOwner) {}
	virtual ~BubblePopState() = default;

	void Enter() override;
	std::unique_ptr<BaseState> Update(float deltaTime) override;
	void HandleInput() override {}
	void Exit() override {}
};
