#include "ScoreObserver.h"

#include <string>

#include "TextComponent.h"
#include "ScoreComponent.h"
#include "GameObject.h"

#if USE_STEAMWORKS
#pragma warning(push)
#pragma warning(disable: 4996)
#include <steam_api.h>
#pragma warning(pop)
#endif

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

			CheckWinCondition(score);
		}
	}
}

void ScoreObserver::CheckWinCondition(int score)
{
	if(score >= m_WinScore && !m_GameWon)
	{
		m_GameWon = true;
		
#if USE_STEAMWORKS
		SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
		SteamUserStats()->StoreStats();
#endif
	}
}

