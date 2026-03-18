#pragma once
#include "Command.h"
#include "GameObject.h"
#include <glm/glm.hpp>

class MoveCommand final :
    public Command 
{
public:
	MoveCommand(dae::GameObject* pGameObject, const glm::vec2& direction);
	virtual void Execute(bool notfirstExecute) override;
    void SetSpeed(float speed) { m_Speed = speed; }
private:
	dae::GameObject* m_pGameObject;
	glm::vec2 m_Direction;

	float m_Speed;
};

