#pragma once
#include "BaseState.h"

class IdleState : public BaseState
{
public:
	IdleState(dae::GameObject* pOwner) : BaseState(pOwner) {}

	virtual ~IdleState() = default;
	void Enter() override;
	void Update(float) override {}
	void HandleInput() override {}
	void Exit() override {}
};