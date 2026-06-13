#pragma once

#include "Component.h"
#include <glm/ext/vector_float2.hpp>

class MaitaPlayerComponent final : public dae::Component
{
public:
	explicit MaitaPlayerComponent(dae::GameObject* owner);

	void TrapInBubble();
	void ReleaseFromBubble(const glm::vec2& position);
	void PopFromBubble(const glm::vec2& position);
	bool IsTrapped() const { return m_IsTrapped; }

private:
	bool m_IsTrapped{};
};
