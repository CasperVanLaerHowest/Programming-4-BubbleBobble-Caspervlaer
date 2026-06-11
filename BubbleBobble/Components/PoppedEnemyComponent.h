#pragma once
#include "Component.h"
#include <glm/ext/vector_float2.hpp>
#include <string>

class PoppedEnemyComponent final : public dae::Component
{
public:
	PoppedEnemyComponent(dae::GameObject* owner, const glm::vec2& targetPosition, const std::string& fruitTexture);
	virtual ~PoppedEnemyComponent() = default;

	void Update(float deltaTime) override;

private:
	glm::vec2 m_StartPosition{};
	glm::vec2 m_TargetPosition{};
	std::string m_FruitTexture{};
	float m_ElapsedTime{};
	float m_Duration{ 5.f };
	float m_ArcHeight{ 80.f };
	float m_RotationSpeed{ 720.f };
	bool m_IsFruit{};
};
