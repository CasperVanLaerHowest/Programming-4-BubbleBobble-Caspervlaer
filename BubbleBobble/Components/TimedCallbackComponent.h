#pragma once

#include "Component.h"
#include <functional>

class TimedCallbackComponent final : public dae::Component
{
public:
	using Callback = std::function<void()>;

	TimedCallbackComponent(dae::GameObject* owner, float delay, Callback callback);
	void Update(float deltaTime) override;

private:
	float m_Delay{};
	Callback m_Callback{};
	bool m_HasTriggered{};
};
