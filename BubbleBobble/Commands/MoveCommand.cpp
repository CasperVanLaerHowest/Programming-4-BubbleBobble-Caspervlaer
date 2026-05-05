#include "MoveCommand.h"
#include "TransformComponent.h"
#include "GameTime.h"
#include "../Components/PhysicsComponent.h"

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
	
	auto physics = m_pGameObject->GetComponent<PhysicsComponent>();
	if(physics)
	{
		physics->AddVelocity(m_Direction * m_Speed);
	}
	else
	{
		auto transform = m_pGameObject->GetComponent<dae::TransformComponent>();
		if (transform)
		{
			auto pos = transform->GetLocalPosition();
			transform->SetLocalPosition(pos.x + (m_Direction.x * m_Speed * GameTime::GetInstance().GetDeltaTime()),
										pos.y + (m_Direction.y * m_Speed * GameTime::GetInstance().GetDeltaTime()),
										pos.z);
		}
	}
}