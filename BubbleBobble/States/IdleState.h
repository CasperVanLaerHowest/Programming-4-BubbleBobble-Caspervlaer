#include "BaseState.h"

class IdleState : public BaseState
{
public:
	IdleState() = default;
	virtual ~IdleState() = default;
	void Enter() override {};
	void Update(float deltaTime) override {};
	void HandleInput() override {};
	void Exit() override {};
};