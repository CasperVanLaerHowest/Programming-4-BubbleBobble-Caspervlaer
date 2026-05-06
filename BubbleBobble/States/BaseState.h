#pragma once
#include "GameObject.h"

class BaseState
{
public:
	BaseState(dae::GameObject* pOwner) : m_pOwner(pOwner) {}
	virtual ~BaseState() = default;
	virtual void Enter() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void HandleInput() = 0;
	virtual void Exit() = 0;
protected:
	dae::GameObject* m_pOwner = nullptr;
};