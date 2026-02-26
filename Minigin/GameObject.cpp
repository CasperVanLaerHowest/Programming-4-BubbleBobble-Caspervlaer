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
	const auto& pos = transformComponent->GetWorldPosition();
	const auto& renderComponent = GetComponent<RenderComponent>();
	Texture2D* texture = renderComponent ? renderComponent->GetTexture() : nullptr;
	if (texture)
	{
		Renderer::GetInstance().RenderTexture(*texture, pos.x, pos.y);
	}
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (parent == this)
		return;
	
	if (parent != nullptr)
	{
		for (auto* child : m_pChildren)
		{
			if (child == parent)
				return;
		}
	}

	if (m_pParent == parent)
		return;

	auto* transform = GetComponent<TransformComponent>();
	
	glm::vec3 worldPos{};
	if (keepWorldPosition && transform)
	{
		worldPos = transform->GetWorldPosition();
	}

	if (m_pParent != nullptr)
	{
		auto& siblings = m_pParent->m_pChildren;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
	}

	m_pParent = parent;

	if (m_pParent != nullptr)
	{
		m_pParent->m_pChildren.push_back(this);
	}

	if (transform)
	{
		if (keepWorldPosition)
		{
			if (m_pParent != nullptr)
			{
				auto* parentTransform = m_pParent->GetComponent<TransformComponent>();
				if (parentTransform)
				{
					glm::vec3 parentWorldPos = parentTransform->GetWorldPosition();
					transform->SetLocalPosition(
						worldPos.x - parentWorldPos.x,
						worldPos.y - parentWorldPos.y,
						worldPos.z - parentWorldPos.z
					);
				}
			}
			else
			{
				transform->SetLocalPosition(worldPos.x, worldPos.y, worldPos.z);
			}
		}
		transform->SetDirtyFlag(true);
	}
}

void dae::GameObject::AddChild(GameObject* child)
{
	if (child != nullptr)
	{
		child->SetParent(this,false);
	}
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	if (child != nullptr && child->m_pParent == this)
	{
		child->SetParent(nullptr,false);
	}
}