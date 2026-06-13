#include "TimedCallbackComponent.h"

#include "SceneManager.h"

#include <utility>

TimedCallbackComponent::TimedCallbackComponent(dae::GameObject* owner, float delay, Callback callback)
	: Component(owner)
	, m_Delay{ delay }
	, m_Callback{ std::move(callback) }
{
}

void TimedCallbackComponent::Update(float deltaTime)
{
	if (m_HasTriggered)
		return;

	m_Delay -= deltaTime;
	if (m_Delay > 0.f)
		return;

	m_HasTriggered = true;
	auto callback = m_Callback;
	dae::SceneManager::GetInstance().QueuePostUpdateAction([callback]()
		{
			if (callback)
			{
				callback();
			}
		});
}
