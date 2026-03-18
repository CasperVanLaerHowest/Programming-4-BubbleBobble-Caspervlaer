#include "MoveCommand.h"
#include "TransformComponent.h"
#include "GameTime.h"

MoveCommand::MoveCommand(dae::GameObject* pGameObject, const glm::vec2& direction)
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
	if (transform)
	{
		auto pos = transform->GetLocalPosition();
		float dt = GameTime::GetInstance().GetDeltaTime();
		
		transform->SetLocalPosition(pos.x + (m_Direction.x * m_Speed * dt), 
		                            pos.y + (m_Direction.y * m_Speed * dt), 
		                            pos.z);
	}
}