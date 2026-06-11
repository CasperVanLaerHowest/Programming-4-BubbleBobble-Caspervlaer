#pragma once
#include "BaseState.h"
#include <glm/ext/vector_float2.hpp>

class PlayerDeathState final : public BaseState
{
public:
	PlayerDeathState(dae::GameObject* pOwner, const glm::vec2& respawnPosition);
	virtual ~PlayerDeathState() = default;

	void Enter() override;
	std::unique_ptr<BaseState> Update(float deltaTime) override;
	void HandleInput() override {}
	void Exit() override {}

private:
	glm::vec2 m_RespawnPosition{};
	float m_ElapsedTime{};
	float m_DeathDuration{ 0.9f };
};
