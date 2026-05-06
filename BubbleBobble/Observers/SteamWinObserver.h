#pragma once  
#include "Component.h"
#include "Observer.h"  

class SteamWinObserver final : 
    public dae::Component,
    public dae::Observer
{
public:
    SteamWinObserver(dae::GameObject* owner);
    virtual ~SteamWinObserver() = default;

	void OnNotify(dae::GameObject& entity, dae::Event event) override;
private:
    const int m_WinScore = 500;
    bool m_GameWon = false;
    void CheckWinCondition(int score);
};
