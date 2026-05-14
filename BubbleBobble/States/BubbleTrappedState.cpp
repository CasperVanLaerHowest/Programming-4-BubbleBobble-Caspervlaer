#include "BubbleTrappedState.h"
#include "BubblePopState.h"
#include "GameObject.h"
#include "../Components/BubbleStateComponent.h"
#include "../Components/PhysicsComponent.h"

void BubbleTrappedState::Enter()
{
	auto* bubble = m_pOwner->GetComponent<BubbleStateComponent>();
	auto* physics = m_pOwner->GetComponent<PhysicsComponent>();
	if (!bubble || !physics)
		return;

	physics->SetVelocity({ 0.f, -bubble->GetFloatSpeed() });
}

std::unique_ptr<BaseState> BubbleTrappedState::Update(float)
{
	auto* bubble = m_pOwner->GetComponent<BubbleStateComponent>();
	if (!bubble)
		return nullptr;

	if (bubble->GetElapsedTime() >= bubble->GetLifetime())
		return std::make_unique<BubblePopState>(m_pOwner);

	return nullptr;
}

void BubbleTrappedState::PrepareMovement()
{
	auto* bubble = m_pOwner->GetComponent<BubbleStateComponent>();
	auto* physics = m_pOwner->GetComponent<PhysicsComponent>();
	if (!bubble || !physics)
		return;

	auto velocity = physics->GetVelocity();
	velocity.y = -bubble->GetFloatSpeed();

	if (!m_WasPushedThisFrame)
		velocity.x = 0.f;

	physics->SetVelocity(velocity);
	m_WasPushedThisFrame = false;
}

void BubbleTrappedState::PushSideways(float direction)
{
	auto* bubble = m_pOwner->GetComponent<BubbleStateComponent>();
	auto* physics = m_pOwner->GetComponent<PhysicsComponent>();
	if (!bubble || !physics)
		return;

	auto velocity = physics->GetVelocity();
	velocity.x = direction * bubble->GetPushVelocity();
	velocity.y = -bubble->GetFloatSpeed();
	physics->SetVelocity(velocity);
	m_WasPushedThisFrame = true;
}
