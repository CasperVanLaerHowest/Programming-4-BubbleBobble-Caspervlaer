#include "ScoreObserver.h"

#include <string>

#include "TextComponent.h"
#include "ScoreComponent.h"
#include "GameObject.h"

ScoreObserver::ScoreObserver(dae::GameObject* owner)
	: Component(owner)
{
}

void ScoreObserver::OnNotify(dae::GameObject& entity, dae::Event event)
{
	if (event == dae::Event::ADD_SCORE)
	{
		const auto& scoreComponent = entity.GetComponent<ScoreComponent>();
		if(scoreComponent)
		{
			const int score = scoreComponent->GetScore();
			
			const auto& textComponent = GetOwner()->GetComponent<dae::TextComponent>();
			if(textComponent)
			{
				textComponent->SetText("Score: " + std::to_string(score));
			}
		}
	}
}