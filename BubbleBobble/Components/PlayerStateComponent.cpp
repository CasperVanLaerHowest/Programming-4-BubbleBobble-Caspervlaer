#include "PlayerStateComponent.h"
#include "../States/IdleState.h"

PlayerStateComponent::PlayerStateComponent(dae::GameObject* owner)
	: Component(owner)
{
	ChangeState(std::make_unique<IdleState>(owner));
}

void PlayerStateComponent::Update(float deltaTime)
{
	if (m_pCurrentState)
	{
		auto newState = m_pCurrentState->Update(deltaTime);
		if (newState)
		{
			ChangeState(std::move(newState));
		}
	}
}

void PlayerStateComponent::ChangeState(std::unique_ptr<BaseState> newState)
{
	if (m_pCurrentState)
		m_pCurrentState->Exit();
	m_pCurrentState = std::move(newState);
	if (m_pCurrentState)
		m_pCurrentState->Enter();
}
