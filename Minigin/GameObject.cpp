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

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	// 1. Check if valid (not itself or one of its children)
	if (parent == this)
		return;
	
	if (parent != nullptr)
	{
		// Check if parent is one of our children (would create a cycle)
		for (auto* child : m_pChildren)
		{
			if (child == parent)
				return;
		}
	}
	
	// If same parent, nothing to do
	if (m_pParent == parent)
		return;

	auto* transform = GetComponent<TransformComponent>();
	
	// 2. Update position (before changing parent)
	glm::vec3 worldPos{};
	if (keepWorldPosition && transform)
	{
		// Store current world position
		worldPos = transform->GetPosition();
	}

	// 3. Remove itself from previous parent
	if (m_pParent != nullptr)
	{
		auto& siblings = m_pParent->m_pChildren;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
	}

	// 4. Set the given parent on itself
	m_pParent = parent;

	// 5. Add itself as a child to the given parent
	if (m_pParent != nullptr)
	{
		m_pParent->m_pChildren.push_back(this);
	}

	// Update transform relative position based on new parent
	if (transform)
	{
		if (keepWorldPosition)
		{
			// Calculate new relative position to maintain the same world position
			if (m_pParent != nullptr)
			{
				auto* parentTransform = m_pParent->GetComponent<TransformComponent>();
				if (parentTransform)
				{
					glm::vec3 parentWorldPos = parentTransform->GetPosition();
					transform->SetLocalPosition(
						worldPos.x - parentWorldPos.x,
						worldPos.y - parentWorldPos.y,
						worldPos.z - parentWorldPos.z
					);
				}
			}
			else
			{
				// No parent, relative position becomes the world position
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