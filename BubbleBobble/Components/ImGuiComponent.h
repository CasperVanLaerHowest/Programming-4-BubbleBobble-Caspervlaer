#pragma once
#include "Component.h"

namespace dae
{
	class ImGuiComponent final : public Component
	{
	public:
		static constexpr ComponentTypeID StaticTypeID{ 12 };
		explicit ImGuiComponent(GameObject* owner)
			: Component(owner)
		{
		}
		virtual ~ImGuiComponent() = default;
		ImGuiComponent(const ImGuiComponent& other) = delete;
		ImGuiComponent(ImGuiComponent&& other) = delete;
		ImGuiComponent& operator=(const ImGuiComponent& other) = delete;
		ImGuiComponent& operator=(ImGuiComponent&& other) = delete;
		void FixedUpdate([[maybe_unused]] float fixedTimeStap) override {}
		void Update([[maybe_unused]] float deltatime) override {}
		void Render() const override;
	};
}