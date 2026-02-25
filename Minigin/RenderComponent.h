#pragma once
#include "Component.h"
#include <memory>
#include <Texture2D.h>
#include "ResourceManager.h"

namespace dae
{
	class RenderComponent : public Component
	{
	public:
		static constexpr ComponentTypeID StaticTypeID{ 3 };

		explicit RenderComponent(GameObject* owner)
			: Component(owner)
		{
		}
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		void FixedUpdate([[maybe_unused]] float fixedTimeStap) override {}
		void Update([[maybe_unused]] float deltatime) override {}
		void Render() const override {}

		// Make GetTexture virtual so derived classes can override it
		virtual Texture2D* GetTexture() const { return nullptr; }
	};
}