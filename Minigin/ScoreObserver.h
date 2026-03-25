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
};