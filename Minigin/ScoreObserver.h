#pragma once  
#include "Component.h"
#include "Observer.h"  

class ScoreObserver final : 
    public dae::Component,
    public dae::Observer
{
public:
    static constexpr dae::ComponentTypeID StaticTypeID{ 9 };

	ScoreObserver(dae::GameObject* owner);
    virtual ~ScoreObserver() = default;

	void OnNotify(dae::GameObject& entity, dae::Event event) override;
private:
    const int m_WinScore = 500;
    bool m_GameWon = false;
    void CheckWinCondition(int score);
};