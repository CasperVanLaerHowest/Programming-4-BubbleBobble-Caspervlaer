#pragma once
#include <memory>
#include "Texture2D.h"
#include "ResourceManager.h"
#include "RenderComponent.h"

namespace dae
{
	class TextureComponent final : public RenderComponent
	{
	public:
		explicit TextureComponent(GameObject* owner)
			: RenderComponent(owner)
		{
		}
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		void SetTexture(const std::string& filename) {
			m_TextTexture = ResourceManager::GetInstance().LoadTexture(filename);
		};

		void SetFlipHorizontal(bool flip) { m_FlipHorizontal = flip; }
		bool IsFlippedHorizontal() const { return m_FlipHorizontal; }

		Texture2D* GetTexture() const override { return m_TextTexture.get(); }
		SDL_FlipMode GetFlipMode() const override { return m_FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE; }
	private:
		std::shared_ptr<Texture2D> m_TextTexture{};
		bool m_FlipHorizontal{ false };
	};
}