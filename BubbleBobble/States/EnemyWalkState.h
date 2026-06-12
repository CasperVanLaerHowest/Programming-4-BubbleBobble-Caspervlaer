#pragma once
#include "EnemyBaseState.h"

class EnemyWalkState final : public EnemyBaseState
{
public:
	EnemyWalkState(dae::GameObject* pOwner) : EnemyBaseState(pOwner) {}
	virtual ~EnemyWalkState() = default;

	void Enter() override;
	std::unique_ptr<BaseState> Update(float deltaTime) override;
};
