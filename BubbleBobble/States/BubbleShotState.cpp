#include "BubbleShotState.h"
#include "BubbleDriftState.h"
#include "BubblePopState.h"
#include "GameObject.h"
#include "../Components/BubbleStateComponent.h"
#include "../Components/FacingComponent.h"
#include "../Components/PhysicsComponent.h"

void BubbleShotState::Enter()
{
	auto* bubble = m_pOwner->GetComponent<BubbleStateComponent>();
	auto* physics = m_pOwner->GetComponent<PhysicsComponent>();
	if (!bubble || !physics)
		return;

	auto* facing = m_pOwner->GetComponent<FacingComponent>();
	const float direction = facing && !facing->IsFacingRight() ? -1.f : 1.f;
	physics->SetVelocity({ direction * bubble->GetSpeed(), 0.f });
}

std::unique_ptr<BaseState> BubbleShotState::Update(float)
{
	auto* bubble = m_pOwner->GetComponent<BubbleStateComponent>();
	if (!bubble)
		return nullptr;

	if (bubble->GetElapsedTime() >= bubble->GetLifetime())
		return std::make_unique<BubblePopState>(m_pOwner);

	if (bubble->GetElapsedTime() > bubble->GetInitialMoveTime())
		return std::make_unique<BubbleDriftState>(m_pOwner);

	return nullptr;
}
