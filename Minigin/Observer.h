#pragma once
#include "GameObject.h"

namespace dae
{
	enum class Event
	{
		TAKE_DAMAGE,
		ADD_SCORE,
		PLAYER_DEAD,
		PLAYER_WON,
	};

	class Observer
	{
	private:
	public:
		virtual ~Observer() {}
		virtual void OnNotify(dae::GameObject& entity, Event event) = 0;
	};
}