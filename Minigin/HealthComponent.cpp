#include "HealthComponent.h"

#include "GameObject.h"

HealthComponent::HealthComponent(dae::GameObject* owner, int health)
	: Component(owner)
	, m_Health{ health }
	, m_MaxHealth{ health }
{
}

void HealthComponent::FixedUpdate(float /*fixedTimeStep*/)
{
	if (m_DirtyFlag)
	{
		if (m_Health <= 0)
		{

			Notify(*GetOwner(), dae::Event::PLAYER_DEAD);
		}
		m_DirtyFlag = false;
	}
}

void HealthComponent::TakeDamage()
{
	m_Health -= 1;
	Notify(*GetOwner(), dae::Event::TAKE_DAMAGE);

	m_DirtyFlag = true;
}
