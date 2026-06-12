#include "BubbleStateComponent.h"
#include "../States/BubbleBaseState.h"
#include "../States/BubblePopState.h"
#include "../States/BubbleShotState.h"
#include "../States/BubbleTrappedState.h"
#include "../HelperFunctions/GameObjectFactory.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"

BubbleStateComponent::BubbleStateComponent(dae::GameObject* owner, dae::Scene& scene)
	: Component(owner)
	, m_pScene{ &scene }
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
	UpdateWarningAnimation();

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
	if (dynamic_cast<BubbleTrappedState*>(m_pCurrentState.get()) != nullptr)
		return;

	ChangeState(std::make_unique<BubbleTrappedState>(GetOwner()));

	if (m_IsWarning)
	{
		if (auto* animation = GetOwner()->GetComponent<AnimationComponent>())
		{
			animation->PlayAnimation("TrappedRed");
		}
	}
}

void BubbleStateComponent::Pop()
{
	ChangeState(std::make_unique<BubblePopState>(GetOwner()));
}

bool BubbleStateComponent::PopTrappedIntoFruit()
{
	if (!IsTrapped() || m_pScene == nullptr || GetOwner()->IsDestroyed())
		return false;

	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transform == nullptr)
		return false;

	const auto& position = transform->GetWorldPosition();
	SpawnFruit(*m_pScene, { position.x, position.y });
	GetOwner()->Destroy();
	return true;
}

void BubbleStateComponent::Expire()
{
	if (GetOwner()->IsDestroyed())
		return;

	if (IsTrapped() && m_pScene != nullptr)
	{
		if (auto* transform = GetOwner()->GetComponent<dae::TransformComponent>())
		{
			const auto& position = transform->GetWorldPosition();
			CreateEnemy(*m_pScene, { position.x, position.y });
		}
	}

	GetOwner()->Destroy();
}

bool BubbleStateComponent::IsFloatingUp() const
{
	const auto* bubbleState = GetCurrentBubbleState();
	return bubbleState && bubbleState->IsFloatingUp();
}

bool BubbleStateComponent::IsTrapped() const
{
	return dynamic_cast<BubbleTrappedState*>(m_pCurrentState.get()) != nullptr;
}

BubbleBaseState* BubbleStateComponent::GetCurrentBubbleState() const
{
	return dynamic_cast<BubbleBaseState*>(m_pCurrentState.get());
}

void BubbleStateComponent::UpdateWarningAnimation()
{
	if (m_IsWarning || m_ElapsedTime < m_Lifetime - m_WarningTime)
		return;

	m_IsWarning = true;

	if (auto* animation = GetOwner()->GetComponent<AnimationComponent>())
	{
		animation->PlayAnimation(IsTrapped() ? "TrappedRed" : "BubbleRed");
	}
}
