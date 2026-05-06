#include "CollisionComponent.h"
#include "AnimationComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Texture2D.h"

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
		
	glm::vec2 thisPos = predictedPos + GetOffset();
	glm::vec2 otherPos{ otherTransform->GetWorldPosition().x + otherCollision->GetOffset().x, otherTransform->GetWorldPosition().y + otherCollision->GetOffset().y };
	
	glm::vec2 thisMin = thisPos;
	glm::vec2 thisMax = thisPos + GetSize();
	glm::vec2 otherMin = otherPos;
	glm::vec2 otherMax = otherPos + otherCollision->GetSize();
	
	return (thisMin.x < otherMax.x && thisMax.x > otherMin.x &&
			thisMin.y < otherMax.y && thisMax.y > otherMin.y);
}

glm::vec2 CollisionComponent::GetSize() const
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	glm::vec2 scale{ 1.f, 1.f };

	if (transform != nullptr)
	{
		const auto& transformScale = transform->GetScale();
		scale = { transformScale.x, transformScale.y };
	}

	if (const auto* animationComponent = GetOwner()->GetComponent<AnimationComponent>())
	{
		const auto currentTexture = animationComponent->GetCurrentTexture();
		if (currentTexture != nullptr)
		{
			const auto size = currentTexture->GetSize();
			return { size.x * scale.x, size.y * scale.y };
		}
	}

	if (const auto* textureComponent = GetOwner()->GetComponent<dae::TextureComponent>())
	{
		const auto* texture = textureComponent->GetTexture();
		if (texture != nullptr)
		{
			const auto size = texture->GetSize();
			return { size.x * scale.x, size.y * scale.y };
		}
	}

	if (const auto* textComponent = GetOwner()->GetComponent<dae::TextComponent>())
	{
		const auto* texture = textComponent->GetTexture();
		if (texture != nullptr)
		{
			const auto size = texture->GetSize();
			return { size.x * scale.x, size.y * scale.y };
		}
	}

	return { m_Size.x * scale.x, m_Size.y * scale.y };
}

glm::vec2 CollisionComponent::GetOffset() const
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	glm::vec2 scale{ 1.f, 1.f };

	if (transform != nullptr)
	{
		const auto& transformScale = transform->GetScale();
		scale = { transformScale.x, transformScale.y };
	}

	return { m_Offset.x * scale.x, m_Offset.y * scale.y };
}