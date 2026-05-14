#pragma once
#include "Component.h"
#include <glm/vec2.hpp>

enum class FacingDirection
{
	Left,
	Right
};

class FacingComponent final : public dae::Component
{
public:
	FacingComponent(dae::GameObject* owner);
	virtual ~FacingComponent() = default;

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
