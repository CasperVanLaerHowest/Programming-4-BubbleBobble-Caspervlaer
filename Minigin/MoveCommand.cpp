#include "MoveCommand.h"
#include "TransformComponent.h"
#include "GameTime.h"

MoveCommand::MoveCommand(dae::GameObject* pGameObject, Direction direction)
	: m_pGameObject{ pGameObject }, m_Direction{ direction }
{
	m_Speed = 100.0f;
}

void MoveCommand::Execute(bool notfirstExecute)
{
	if (notfirstExecute)
	{
		//std::cout << "first MoveCommand executed" << std::endl;
	}
	
	auto transform = m_pGameObject->GetComponent<dae::TransformComponent>();
	auto pos = transform->GetLocalPosition();
	

	switch (m_Direction)
	{
	case Direction::UP:
		transform->SetLocalPosition(pos.x, pos.y - (m_Speed * GameTime::GetInstance().GetDeltaTime()), pos.z);
		break;
	case Direction::DOWN:
		transform->SetLocalPosition(pos.x, pos.y + (m_Speed * GameTime::GetInstance().GetDeltaTime()), pos.z);
		break;
	case Direction::LEFT:
		transform->SetLocalPosition(pos.x - (m_Speed * GameTime::GetInstance().GetDeltaTime()), pos.y, pos.z);
		break;
	case Direction::RIGHT:
		transform->SetLocalPosition(pos.x + (m_Speed * GameTime::GetInstance().GetDeltaTime()), pos.y, pos.z);
		break;
	case Direction::NONE:
		break;
	}
}