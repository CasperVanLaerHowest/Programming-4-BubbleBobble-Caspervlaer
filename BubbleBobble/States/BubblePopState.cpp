#include "BubblePopState.h"
#include "GameObject.h"
#include "../Components/PhysicsComponent.h"

void BubblePopState::Enter()
{
	if (auto* physics = m_pOwner->GetComponent<PhysicsComponent>())
	{
		physics->SetVelocity({ 0.f, 0.f });
	}
}

std::unique_ptr<BaseState> BubblePopState::Update(float)
{
	return nullptr;
}
