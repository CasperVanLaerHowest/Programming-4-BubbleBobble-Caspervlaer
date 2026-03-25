#include "SteamWinObserver.h"

#include "ScoreComponent.h"
#include "GameObject.h"

#if USE_STEAMWORKS
#pragma warning(push)
#pragma warning(disable: 4996)
#include <steam_api.h>
#pragma warning(pop)
#endif

SteamWinObserver::SteamWinObserver(dae::GameObject* owner)
	: Component(owner)
{
}

void SteamWinObserver::OnNotify(dae::GameObject& entity, dae::Event event)
{
	if (event == dae::Event::ADD_SCORE)
	{
		const auto& scoreComponent = entity.GetComponent<ScoreComponent>();
		if(scoreComponent)
		{
			CheckWinCondition(scoreComponent->GetScore());
		}
	}
}

void SteamWinObserver::CheckWinCondition(int score)
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
