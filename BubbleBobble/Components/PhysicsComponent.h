#pragma once
#include "Component.h"
#include <glm/ext/vector_float2.hpp>

class PhysicsComponent : public dae::Component
{
public:
	PhysicsComponent(dae::GameObject* owner);
	virtual ~PhysicsComponent() = default;
	void FixedUpdate(float) override {};
	void Update(float) override;
	void Render() const override {};

	void AddVelocity(const glm::vec2& velocity) { m_Velocity += velocity; }
	const glm::vec2& GetVelocity() const { return m_Velocity; }
	void SetGravity(float gravity) { m_Gravity = gravity; }
	void ResetGravity() { m_Gravity = 300.f; }

	bool IsGrounded() const;

private:
	glm::vec2 m_Velocity{ 0.f, 0.f };
	float m_Gravity{ 500.f };

	float m_MaxVerticalSpeed{ 300.f };
	float m_MaxHorizontalSpeed{ 100.f };
	float m_GroundVelocityEpsilon{ 1.f };
	float m_GroundStopVelocity{ 5.f };
	float m_GroundFriction{ 1200.f };
	float m_AirFriction{ 50.f };
};