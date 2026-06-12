#pragma once
#include "EnemyBaseState.h"

class EnemyAngryState final : public EnemyBaseState
{
public:
	EnemyAngryState(dae::GameObject* pOwner) : EnemyBaseState(pOwner) {}
	virtual ~EnemyAngryState() = default;

	void Enter() override;
	std::unique_ptr<BaseState> Update(float deltaTime) override;
};
