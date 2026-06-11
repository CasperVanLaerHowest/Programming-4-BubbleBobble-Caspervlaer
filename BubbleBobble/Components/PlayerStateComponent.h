#pragma once
#include "Component.h"
#include "../States/BaseState.h"
#include <memory>
#include <glm/ext/vector_float2.hpp>

class PlayerStateComponent : public dae::Component
{
public:
	PlayerStateComponent(dae::GameObject* owner, const glm::vec2& spawnPosition);
	virtual ~PlayerStateComponent() = default;
	void FixedUpdate(float) override {};
	void Update(float) override;
	void Render() const override {};

	void ChangeState(std::unique_ptr<BaseState> newState);
	void TakeHit();
	bool IsDying() const;
	const glm::vec2& GetSpawnPosition() const { return m_SpawnPosition; }

private:
	std::unique_ptr<BaseState> m_pCurrentState;
	glm::vec2 m_SpawnPosition{};
};
