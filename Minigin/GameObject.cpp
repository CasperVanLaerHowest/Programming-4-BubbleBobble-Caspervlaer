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
	const auto& pos = transformComponent->GeLocalPosition();
	const auto& renderComponent = GetComponent<RenderComponent>();
	Texture2D* texture = renderComponent ? renderComponent->GetTexture() : nullptr;
	if (texture)
	{
		Renderer::GetInstance().RenderTexture(*texture, pos.x, pos.y);
	}
}

void dae::GameObject::SetParent(GameObject* parent)
{
	if (parent == this)
		return;

	for (auto childExist : m_pChildren)
	{
		if(childExist == parent){
			return;
		}
	}

	if (m_pParent != nullptr)
	{
		m_pParent->RemoveChild(this);
	}

	m_pParent = parent;

	m_pParent->m_pChildren.emplace_back(this);

	// update my world position based on my new parent
}

void dae::GameObject::AddChild(GameObject* child)
{
	if (child == nullptr)
		return;
	if (child == this)
		return;
	if (m_pParent == child)
		return;

	if(child->GetParent() != nullptr)
	{
		child->GetParent()->RemoveChild(child);
	}

	child->m_pParent = this;

	m_pChildren.push_back(child);

	// update child's world position based on my world position
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	if (child == nullptr)
		return;
	if (child == this)
		return;
	
	for (auto childExist : m_pChildren)
	{
		if(childExist == child)
		{
			m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), child), m_pChildren.end());
			child->SetParent(nullptr);
			// update child's world position based on my world position
		}
	}
}