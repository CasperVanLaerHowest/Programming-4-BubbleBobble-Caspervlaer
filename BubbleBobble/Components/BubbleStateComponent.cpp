#include "BubbleStateComponent.h"
#include "../States/BubbleBaseState.h"
#include "../States/BubblePopState.h"
#include "../States/BubbleShotState.h"
#include "../States/BubbleTrappedState.h"
#include "GameObject.h"
#include "PhysicsComponent.h"

BubbleStateComponent::BubbleStateComponent(dae::GameObject* owner)
	: Component(owner)
{
	auto physics = GetOwner()->GetComponent<PhysicsComponent>();
	if (physics)
	{
		physics->SetGravity(0.f);
		physics->SetMaxHorizontalSpeed(m_Speed);
		physics->SetMaxVerticalSpeed(m_Speed);
		physics->SetFriction(0.f, 0.f);
	}
}

void BubbleStateComponent::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;

	if (!m_pCurrentState)
		return;

	auto newState = m_pCurrentState->Update(deltaTime);
	if (newState)
	{
		ChangeState(std::move(newState));
	}
}

void BubbleStateComponent::Start()
{
	if (!m_pCurrentState)
	{
		ChangeState(std::make_unique<BubbleShotState>(GetOwner()));
	}
}

void BubbleStateComponent::ChangeState(std::unique_ptr<BaseState> newState)
{
	if (m_pCurrentState)
		m_pCurrentState->Exit();

	m_pCurrentState = std::move(newState);

	if (m_pCurrentState)
		m_pCurrentState->Enter();
}

void BubbleStateComponent::PrepareMovement()
{
	if (auto* bubbleState = GetCurrentBubbleState())
	{
		bubbleState->PrepareMovement();
	}
}

void BubbleStateComponent::PushSideways(float direction)
{
	if (auto* bubbleState = GetCurrentBubbleState())
	{
		bubbleState->PushSideways(direction);
	}
}

void BubbleStateComponent::TrapEnemy()
{
	ChangeState(std::make_unique<BubbleTrappedState>(GetOwner()));
}

void BubbleStateComponent::Pop()
{
	ChangeState(std::make_unique<BubblePopState>(GetOwner()));
}

bool BubbleStateComponent::IsFloatingUp() const
{
	const auto* bubbleState = GetCurrentBubbleState();
	return bubbleState && bubbleState->IsFloatingUp();
}

BubbleBaseState* BubbleStateComponent::GetCurrentBubbleState() const
{
	return dynamic_cast<BubbleBaseState*>(m_pCurrentState.get());
}
