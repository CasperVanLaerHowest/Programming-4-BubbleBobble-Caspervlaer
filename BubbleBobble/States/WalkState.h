#pragma once
#include "BaseState.h"

class WalkState : public BaseState
{
	public:
	WalkState(dae::GameObject* pOwner) : BaseState(pOwner) {}
	virtual ~WalkState() = default;
	void Enter() override;
	std::unique_ptr<BaseState> Update(float deltaTime) override;
	void HandleInput() override {};
	void Exit() override {};
};
