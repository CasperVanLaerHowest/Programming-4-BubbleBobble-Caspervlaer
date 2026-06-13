#pragma once

#include "Command.h"
#include "GameObject.h"
#include "Scene.h"

class ShootMaitaBallCommand final : public Command
{
public:
	ShootMaitaBallCommand(dae::GameObject* pGameObject, dae::Scene& scene);
	void Execute(bool notfirstExecute) override;

private:
	dae::GameObject* m_pGameObject{};
	dae::Scene& m_Scene;
};
