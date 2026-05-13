#pragma once
#include "Component.h"
#include <glm/vec2.hpp>

enum class FacingDirection
{
	Left,
	Right
};

class PlayerFacingComponent final : public dae::Component
{
public:
	PlayerFacingComponent(dae::GameObject* owner);
	virtual ~PlayerFacingComponent() = default;

	void FixedUpdate(float) override {}
	void Update(float) override {}
	void Render() const override {}

	void SetFacingDirection(FacingDirection direction);
	void SetFacingFromHorizontalMovement(float horizontalMovement);

	FacingDirection GetFacingDirection() const;
	glm::vec2 GetFacingVector() const;
	bool IsFacingRight() const;

private:
	FacingDirection m_FacingDirection{ FacingDirection::Left };
};
