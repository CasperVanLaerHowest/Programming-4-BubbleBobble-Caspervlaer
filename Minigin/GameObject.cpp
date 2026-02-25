#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

dae::GameObject::GameObject() {
	AddComponent<TransformComponent>();
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::FixedUpdate([[maybe_unused]] float fixedTimeStap) {
	for (const auto& component : m_components)
	{
		if (component != nullptr)
		{
			component->FixedUpdate(fixedTimeStap);
		}
	}
}

void dae::GameObject::Update([[maybe_unused]] float deltaTime){
	for (const auto& component : m_components)
	{
		if (component != nullptr)
		{
			component->Update(deltaTime);
		}
	}
}

void dae::GameObject::Render() const
{
	const auto& transformComponent = GetComponent<TransformComponent>();
	const auto& pos = transformComponent->GetPosition();
	const auto& renderComponent = GetComponent<RenderComponent>();
	Texture2D* texture = renderComponent ? renderComponent->GetTexture() : nullptr;
	if (texture)
	{
		Renderer::GetInstance().RenderTexture(*texture, pos.x, pos.y);
	}
}