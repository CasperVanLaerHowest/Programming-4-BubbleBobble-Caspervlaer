#pragma once
#include "BaseState.h"

class BubbleBaseState : public BaseState
{
public:
	BubbleBaseState(dae::GameObject* pOwner) : BaseState(pOwner) {}
	virtual ~BubbleBaseState() = default;

	virtual void PrepareMovement() {}
	virtual void PushSideways(float) {}
	virtual bool IsFloatingUp() const { return false; }
};
