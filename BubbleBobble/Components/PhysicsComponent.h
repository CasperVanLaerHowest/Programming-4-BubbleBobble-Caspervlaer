#pragma once
#include "Component.h"
#include <glm/ext/vector_float2.hpp>

class PhysicsComponent : public dae::Component
{
public:
	static constexpr dae::ComponentTypeID StaticTypeID{ 11 };

	PhysicsComponent(dae::GameObject* owner);
	virtual ~PhysicsComponent() = default;
	void FixedUpdate(float) override {};
	void Update(float) override;
	void Render() const override {};

	void AddVelocity(const glm::vec2& velocity) { m_Velocity += velocity; }

private:
	glm::vec2 m_Velocity{ 0.f, 0.f };
	float m_Gravity{ 98.1f };

	float m_MaxVerticalSpeed{ 200.f };
	float m_MaxHorizontalSpeed{ 100.f };
};