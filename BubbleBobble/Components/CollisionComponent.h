#pragma once
#include "Component.h"
#include <glm/ext/vector_float2.hpp>
#include <vector>

enum class CollisionType
{
	Player,
	Enemy,
	Platform,
	Solid,
	Bubble,
	Fruit
};

class CollisionComponent : public dae::Component
{
public:
	CollisionComponent(dae::GameObject* owner, const glm::vec2& size, const CollisionType& collisionType, const glm::vec2& offset = glm::vec2{ 0.f, 0.f });
	virtual ~CollisionComponent();
	void FixedUpdate(float) override {};
	void Update(float) override {};
	void Render() const override {};

	bool CheckCollision(const glm::vec2& predictedPos, const dae::GameObject* otherObject) const;

	glm::vec2 GetSize() const;
	glm::vec2 GetOffset() const;

	static const std::vector<CollisionComponent*>& GetColliders() { return m_Colliders; }

	CollisionType GetCollisionType() const { return m_CollisionType; }

private:
	glm::vec2 m_Size{ 0.f, 0.f };
	glm::vec2 m_Offset{ 0.f, 0.f };

	static inline std::vector<CollisionComponent*> m_Colliders;

	CollisionType m_CollisionType{ CollisionType::Platform };
};