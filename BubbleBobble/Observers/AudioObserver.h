#pragma once

#include "Component.h"
#include "Observer.h"

class AudioObserver final :
	public dae::Component,
	public dae::Observer
{
public:
	explicit AudioObserver(dae::GameObject* owner);
	~AudioObserver() override = default;

	void OnNotify(dae::GameObject& entity, dae::Event event) override;
};
