#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

CollisionComponent::CollisionComponent(dae::GameObject* owner, const glm::vec2& size, const glm::vec2& offset)
	: Component(owner), m_Size(size), m_Offset(offset)
{
	m_Colliders.push_back(this);
}

CollisionComponent::~CollisionComponent()
{
	m_Colliders.erase(std::remove(m_Colliders.begin(), m_Colliders.end(), this), m_Colliders.end());
}

bool CollisionComponent::CheckCollision(const glm::vec2& predictedPos, const dae::GameObject* otherObject) const
{
	auto otherTransform = otherObject->GetComponent<dae::TransformComponent>();
	auto otherCollision = otherObject->GetComponent<CollisionComponent>();
	
	if (otherTransform == nullptr || otherCollision == nullptr)
		return false;
		
	// Use predictedPos instead of getting the current world position to check future collision
	glm::vec2 thisPos = predictedPos + m_Offset;
	glm::vec2 otherPos{ otherTransform->GetWorldPosition().x + otherCollision->GetOffset().x, otherTransform->GetWorldPosition().y + otherCollision->GetOffset().y };
	
	glm::vec2 thisMin = thisPos;
	glm::vec2 thisMax = thisPos + m_Size;
	glm::vec2 otherMin = otherPos;
	glm::vec2 otherMax = otherPos + otherCollision->GetSize();
	
	return (thisMin.x < otherMax.x && thisMax.x > otherMin.x &&
			thisMin.y < otherMax.y && thisMax.y > otherMin.y);
}