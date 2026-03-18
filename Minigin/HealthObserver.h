#pragma once  
#include "Component.h"
#include "Observer.h"  
class HealthObserver :  
	public dae::Component, 
	public dae::Observer  
{  
public:  
    static constexpr dae::ComponentTypeID StaticTypeID{ 6 };

   HealthObserver(dae::GameObject* owner);  
   virtual ~HealthObserver() = default;  

   virtual void OnNotify(dae::GameObject& entity, dae::Event event) override;
};