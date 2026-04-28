#pragma once
#include "Component.h"

namespace dae
{
	class FPSComponent final : public Component
	{
	public:
		static constexpr ComponentTypeID StaticTypeID{ 4 };

		explicit FPSComponent(GameObject* owner)
			: Component(owner)
		{
		}
		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		void FixedUpdate(float /*fixedTimeStap*/) override {}
		void Update(float deltatime) override;
		void Render() const override {}
	private:
		float m_currentFPS{ 0.f };
		float fpsTimer{ 0.f };
		int frameCount{ 0 };

		void UpdateFPS();
		void CalculateFPS(float deltaTime);
	};
}