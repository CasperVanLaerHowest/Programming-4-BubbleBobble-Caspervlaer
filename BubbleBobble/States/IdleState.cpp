#include "Idlestate.h"
#include "GameObject.h"
#include "../Components/AnimationComponent.h"

void IdleState::Enter()
{
	m_pOwner->GetComponent<AnimationComponent>()->PlayAnimation("idle");
}