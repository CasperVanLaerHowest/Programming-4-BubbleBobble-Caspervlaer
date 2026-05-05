#pragma once
#include "Component.h"
#include "../States/BaseState.h"
#include <memory>

class PlayerStateComponent : public dae::Component
{
	public:
	static constexpr dae::ComponentTypeID StaticTypeID{ 14 };
	PlayerStateComponent(dae::GameObject* owner);
	virtual ~PlayerStateComponent() = default;
	void FixedUpdate(float) override {};
	void Update(float) override;
	void Render() const override {};

	void ChangeState(std::unique_ptr<BaseState> newState);

private:
	std::unique_ptr<BaseState> m_pCurrentState;
};