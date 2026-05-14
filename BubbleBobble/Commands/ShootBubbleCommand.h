#pragma once
#include "Command.h"
#include "GameObject.h"
#include "Scene.h"
#include <glm/glm.hpp>

class ShootBubbleCommand final :
	public Command
{
public:
	ShootBubbleCommand(dae::GameObject* pGameObject, dae::Scene& scene);
	virtual void Execute(bool notfirstExecute) override;
private:
	dae::GameObject* m_pGameObject;
	dae::Scene& m_Scene;
};