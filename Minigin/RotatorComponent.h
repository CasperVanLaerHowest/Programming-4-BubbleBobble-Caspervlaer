#pragma once
#include "Component.h"

namespace dae
{
	class RotatorComponent final : public Component
	{
	public:
		static constexpr ComponentTypeID StaticTypeID{ 5 };

		explicit RotatorComponent(GameObject* owner, float radius = 50.f, float rotationSpeed = 90.f)
			: Component(owner)
			, m_Radius(radius)
			, m_RotationSpeed(rotationSpeed)
		{
		}
		virtual ~RotatorComponent() = default;
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		void FixedUpdate(float /*fixedTimeStep*/) override {}
		void Update(float deltaTime) override;
		void Render() const override {}

		void SetRadius(float radius) { m_Radius = radius; }
		void SetRotationSpeed(float degreesPerSecond) { m_RotationSpeed = degreesPerSecond; }

	private:
		float m_Radius;          // Distance from the center (0,0,0)
		float m_RotationSpeed;   // Degrees per second
		float m_CurrentAngle{ 0.f };
	};
}