#pragma once
#include <string>
#include <memory>
#include "Font.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "Texture2D.h"
#include "RenderComponent.h"

namespace dae
{
	class TextComponent final : public RenderComponent
	{
	public:
		static constexpr ComponentTypeID StaticTypeID{ 1 };

		TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });

		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void FixedUpdate([[maybe_unused]] float fixedTimeStap) override {}
		void Update([[maybe_unused]] float deltatime) override;
		void Render() const override {}

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

		Texture2D* GetTexture() const override { return m_TextTexture.get(); }
	private:
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font{};

		bool m_needsUpdate{true};

		std::unique_ptr<Texture2D> m_TextTexture;

		void UpdateTexture();
	};
}