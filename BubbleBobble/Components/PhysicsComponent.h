#pragma once
#include "Component.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include "CollisionComponent.h"

class PhysicsComponent : public dae::Component
{
public:
	PhysicsComponent(dae::GameObject* owner);
	virtual ~PhysicsComponent() = default;
	void FixedUpdate(float) override {};
	void Update(float) override;
	void Render() const override {};

	void AddVelocity(const glm::vec2& velocity) { m_Velocity += velocity; }
	void SetVelocity(const glm::vec2& velocity) { m_Velocity = velocity; }
	const glm::vec2& GetVelocity() const { return m_Velocity; }
	void SetGravity(float gravity) { m_Gravity = gravity; }
	void ResetGravity() { m_Gravity = 500.f; }
	void SetMaxHorizontalSpeed(float speed) { m_MaxHorizontalSpeed = speed; }
	void SetMaxVerticalSpeed(float speed) { m_MaxVerticalSpeed = speed; }
	void SetFriction(float groundFriction, float airFriction) { m_GroundFriction = groundFriction; m_AirFriction = airFriction; }

	bool IsGrounded() const;

private:
	glm::vec2 m_Velocity{ 0.f, 0.f };
	float m_Gravity{ 500.f };

	float m_MaxVerticalSpeed{ 320.f };
	float m_MaxHorizontalSpeed{ 100.f };
	float m_GroundVelocityEpsilon{ 1.f };
	float m_GroundStopVelocity{ 5.f };
	float m_GroundFriction{ 1200.f };
	float m_AirFriction{ 50.f };

	void SetCorrectVelocity(float deltaTime);
	void CollisionCheck(float deltaTime, glm::vec3 pos, CollisionComponent* collider);
	void HandleBubbleInteraction(CollisionComponent* collider, CollisionComponent* otherCollider, const glm::vec2& currentPosition, const glm::vec2& predictedPosX, const glm::vec2& predictedPosY);
};
