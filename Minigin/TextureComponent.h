#pragma once
#include <memory>
#include <Texture2D.h>
#include "ResourceManager.h"
#include "RenderComponent.h"

namespace dae
{
	class TextureComponent final : public RenderComponent
	{
	public:
		static constexpr ComponentTypeID StaticTypeID{ 2 };

		explicit TextureComponent(GameObject* owner)
			: RenderComponent(owner)
		{
		}
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		void FixedUpdate(float /*fixedTimeStap*/) override {}
		void Update(float /*deltatime*/) override {}
		void Render() const override {}

		void SetTexture(const std::string& filename) {
			m_TextTexture = ResourceManager::GetInstance().LoadTexture(filename);
		};

		Texture2D* GetTexture() const override { return m_TextTexture.get(); }
	private:
		std::shared_ptr<Texture2D> m_TextTexture{};
	};
}