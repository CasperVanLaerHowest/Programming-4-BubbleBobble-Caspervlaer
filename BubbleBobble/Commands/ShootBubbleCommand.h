#pragma once
#include "Command.h"
#include "GameObject.h"
#include <glm/glm.hpp>

class ShootBubbleCommand final :
	public Command
{
public:
	ShootBubbleCommand(dae::GameObject* pGameObject);
	virtual void Execute(bool notfirstExecute) override;
private:
	dae::GameObject* m_pGameObject;
};