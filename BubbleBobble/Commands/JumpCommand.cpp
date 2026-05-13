#include "JumpCommand.h"
#include "TransformComponent.h"
#include "../Components/PhysicsComponent.h"

JumpCommand::JumpCommand(dae::GameObject* pGameObject, float jumpStrength)
	: m_pGameObject(pGameObject)
	, m_JumpStrength(jumpStrength)
{
}

void JumpCommand::Execute(bool notfirstExecute)
{
	auto physics = m_pGameObject->GetComponent<PhysicsComponent>();
	if (physics && physics->IsGrounded() && !notfirstExecute)
	{
		physics->AddVelocity({ 0.f, -m_JumpStrength });
	}
}