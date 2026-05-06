#pragma once
#include "Command.h"
#include "GameObject.h"

class JumpCommand final :
	public Command
{
public:
	JumpCommand(dae::GameObject* pGameObject, float jumpStrength = 200.f);
	virtual void Execute(bool notfirstExecute) override;
	void SetJumpStrength(float jumpStrength) { m_JumpStrength = jumpStrength; }

private:
	dae::GameObject* m_pGameObject;
	float m_JumpStrength;
};