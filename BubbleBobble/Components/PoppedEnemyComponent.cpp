#include "PoppedEnemyComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include <algorithm>
#include <cmath>
#include <glm/geometric.hpp>

namespace
{
	constexpr float Pi{ 3.1415926535f };
}

PoppedEnemyComponent::PoppedEnemyComponent(dae::GameObject* owner, const glm::vec2& targetPosition, const std::string& fruitTexture, int scoreValue)
	: Component(owner)
	, m_TargetPosition{ targetPosition }
	, m_FruitTexture{ fruitTexture }
	, m_ScoreValue{ scoreValue }
{
	if (auto* transform = GetOwner()->GetComponent<dae::TransformComponent>())
	{
		const auto& position = transform->GetLocalPosition();
		m_StartPosition = { position.x, position.y };
	}

	const float distance = glm::length(m_TargetPosition - m_StartPosition);
	m_Duration = std::max(0.35f, distance / 220.f);
}

void PoppedEnemyComponent::Update(float deltaTime)
{
	if (m_IsFruit)
		return;

	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transform == nullptr)
		return;

	m_ElapsedTime += deltaTime;
	const float t = std::clamp(m_ElapsedTime / m_Duration, 0.f, 1.f);
	const glm::vec2 linearPosition = m_StartPosition + (m_TargetPosition - m_StartPosition) * t;
	const float arcOffset = -std::sin(t * Pi) * m_ArcHeight;

	transform->SetLocalPosition(linearPosition.x, linearPosition.y + arcOffset, 0.f);
	transform->SetRotation(0.f, 0.f, m_ElapsedTime * m_RotationSpeed);

	if (t < 1.f)
		return;

	transform->SetLocalPosition(m_TargetPosition.x, m_TargetPosition.y, 0.f);
	transform->SetRotation(0.f, 0.f, 0.f);

	if (auto* texture = GetOwner()->GetComponent<dae::TextureComponent>())
	{
		texture->SetTexture(m_FruitTexture);
	}

	GetOwner()->AddComponent<CollisionComponent>(
		glm::vec2{ 16.f, 16.f },
		CollisionType::Fruit
	);

	m_IsFruit = true;
}
