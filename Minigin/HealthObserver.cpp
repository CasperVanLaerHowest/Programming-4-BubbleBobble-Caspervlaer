#include "HealthObserver.h"

#include <string>

#include "TextComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"

HealthObserver::HealthObserver(dae::GameObject* owner)
	: Component(owner)
{
}

void HealthObserver::OnNotify(dae::GameObject& entity, dae::Event event)
{
	switch (event)
	{
		case dae::Event::TAKE_DAMAGE:
		{
			GetOwner()->GetComponent<dae::TextComponent>()
				->SetText("Health: " + std::to_string(entity.GetComponent<HealthComponent>()
					->GetHealth()));
			break;
		}
		case dae::Event::PLAYER_DEAD:
		{
			GetOwner()->GetComponent<dae::TextComponent>()
				->SetText("Player Dead");
			break;
		}
	}
}