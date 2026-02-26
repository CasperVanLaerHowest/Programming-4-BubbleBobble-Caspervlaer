#include "TransformComponent.h"
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* owner)
	: Component(owner)
{
}

#pragma region Position
void dae::TransformComponent::SetLocalPosition(float x, float y, float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	m_IsDirty = true;
}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_IsDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldPosition;
}

void dae::TransformComponent::UpdateWorldPosition()
{
	GameObject* parent = GetOwner()->GetParent();
	if (parent == nullptr)
	{
		// No parent, world position equals local position
		m_WorldPosition = m_LocalPosition;
	}
	else
	{
		// World position = parent's world position + our local position
		auto* parentTransform = parent->GetComponent<TransformComponent>();
		if (parentTransform)
		{
			m_WorldPosition = parentTransform->GetWorldPosition() + m_LocalPosition;
		}
	}
	m_IsDirty = false;
}
#pragma endregion Position

#pragma region Scale
void dae::TransformComponent::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

const glm::vec3& dae::TransformComponent::GetScale()
{
	return m_Scale;
}
#pragma endregion Scale

#pragma region Rotation
void dae::TransformComponent::SetRotation(float x, float y, float z)
{
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;
}

const glm::vec3& dae::TransformComponent::GetRotation()
{
	return m_Rotation;
}
#pragma endregion Rotation