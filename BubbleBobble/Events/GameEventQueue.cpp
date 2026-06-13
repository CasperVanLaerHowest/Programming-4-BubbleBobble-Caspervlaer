#include "GameEventQueue.h"

#include "SceneManager.h"

#include <utility>

GameEventQueue& GameEventQueue::GetInstance()
{
	static GameEventQueue instance{};
	return instance;
}

void GameEventQueue::QueueEvent(const GameEvent& event)
{
	m_Events.push_back(event);
	if (m_DispatchQueued)
		return;

	m_DispatchQueued = true;
	dae::SceneManager::GetInstance().QueuePostUpdateAction([]()
		{
			GameEventQueue::GetInstance().Dispatch();
		});
}

void GameEventQueue::Subscribe(EventHandler handler)
{
	m_Handlers.push_back(std::move(handler));
}

void GameEventQueue::ClearSubscribers()
{
	m_Handlers.clear();
}

void GameEventQueue::Dispatch()
{
	m_DispatchQueued = false;
	auto events = std::move(m_Events);
	m_Events.clear();

	for (const auto& event : events)
	{
		for (const auto& handler : m_Handlers)
		{
			if (handler)
			{
				handler(event);
			}
		}
	}
}
