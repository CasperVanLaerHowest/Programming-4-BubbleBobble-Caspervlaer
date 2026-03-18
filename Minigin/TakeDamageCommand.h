#pragma once
#include "Command.h"
#include "GameObject.h"
#include "HealthComponent.h"

class TakeDamageCommand final :
    public Command 
{
public:
	TakeDamageCommand(dae::GameObject* pGameObject);
	virtual void Execute(bool notfirstExecute) override;
private:
	dae::GameObject* m_pGameObject;
};