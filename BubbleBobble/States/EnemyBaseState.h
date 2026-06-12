#pragma once
#include "BaseState.h"

class EnemyBaseState : public BaseState
{
public:
	EnemyBaseState(dae::GameObject* pOwner) : BaseState(pOwner) {}
	virtual ~EnemyBaseState() = default;

	void HandleInput() override {}
	void Exit() override {}
};
