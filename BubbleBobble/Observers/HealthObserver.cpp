#include "HealthObserver.h"

#include <algorithm>

#include "../Components/HealthComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "TransformComponent.h"

HealthObserver::HealthObserver(dae::GameObject* owner, const std::string& textureFile, int startHealth)
	: Component(owner)
	, m_LifeTexture{ dae::ResourceManager::GetInstance().LoadTexture(textureFile) }
{
	SetHealth(startHealth);
}

void HealthObserver::OnNotify(dae::GameObject& entity, dae::Event event)
{
	switch (event)
	{
		case dae::Event::TAKE_DAMAGE:
		{
			SetHealth(entity.GetComponent<HealthComponent>()->GetHealth());
			break;
		}
		case dae::Event::PLAYER_DEAD:
		{
			SetHealth(0);
			break;
		}
	}
}

void HealthObserver::Render() const
{
	if (m_LifeTexture == nullptr || m_VisibleLives <= 0)
	{
		return;
	}

	auto* transformComponent = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transformComponent == nullptr)
	{
		return;
	}

	const auto& pos = transformComponent->GetWorldPosition();
	const auto& scale = transformComponent->GetScale();
	const auto size = m_LifeTexture->GetSize();
	const float width = size.x * scale.x;
	const float height = size.y * scale.y;

	for (int lifeIndex{}; lifeIndex < m_VisibleLives; ++lifeIndex)
	{
		const float x = pos.x + lifeIndex * (width + m_Spacing);
		dae::Renderer::GetInstance().RenderTexture(*m_LifeTexture, x, pos.y, width, height);
	}
}

void HealthObserver::SetHealth(int health)
{
	m_VisibleLives = std::max(0, health - 1);
}
