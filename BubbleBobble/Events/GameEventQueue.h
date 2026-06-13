#pragma once

#include "GameEvent.h"

#include <functional>
#include <vector>

class GameEventQueue final
{
public:
	using EventHandler = std::function<void(const GameEvent&)>;

	static GameEventQueue& GetInstance();

	void QueueEvent(const GameEvent& event);
	void Subscribe(EventHandler handler);
	void ClearSubscribers();

private:
	GameEventQueue() = default;

	void Dispatch();

	std::vector<GameEvent> m_Events{};
	std::vector<EventHandler> m_Handlers{};
	bool m_DispatchQueued{};
};
