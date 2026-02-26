#include "RotatorComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace dae
{
	void RotatorComponent::Update(float deltaTime)
	{
		// Update angle (convert speed from degrees to radians)
		m_CurrentAngle += glm::radians(m_RotationSpeed) * deltaTime;
		
		// Keep angle in [0, 2π] range
		if (m_CurrentAngle > glm::two_pi<float>())
			m_CurrentAngle -= glm::two_pi<float>();

		// Calculate new local position on the circle
		const float x = m_Radius * std::cos(m_CurrentAngle);
		const float y = m_Radius * std::sin(m_CurrentAngle);

		// Update the transform's local position
		if (auto* transform = GetOwner()->GetComponent<TransformComponent>())
		{
			transform->SetLocalPosition(x, y, 0.f);
		}
	}
}