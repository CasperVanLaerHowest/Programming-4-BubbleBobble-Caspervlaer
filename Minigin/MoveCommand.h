#pragma once
#include "Command.h"
#include "GameObject.h"

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};

class MoveCommand final :
    public Command 
{
public:
	MoveCommand(dae::GameObject* pGameObject, Direction direction);
	virtual void Execute(bool notfirstExecute) override;
	//void SetDirection(glm::vec3& direction) { m_Direction = direction; }
    void SetSpeed(float speed) { m_Speed = speed; }
private:
	dae::GameObject* m_pGameObject;
	Direction m_Direction;

	float m_Speed;
};

