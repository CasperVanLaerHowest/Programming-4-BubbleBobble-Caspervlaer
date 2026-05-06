#pragma once
#include "BaseState.h"

class WalkState : public BaseState
{
	public:
	WalkState(dae::GameObject* pOwner) : BaseState(pOwner) {}
	virtual ~WalkState() = default;
	void Enter() override;
	void Update(float deltaTime) override;
	void HandleInput() override {};
	void Exit() override {};
};