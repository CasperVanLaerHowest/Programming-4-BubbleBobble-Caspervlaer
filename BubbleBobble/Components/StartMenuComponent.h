#pragma once

#include "../Game/GameMode.h"
#include "Component.h"

#include <array>
#include <cstddef>
#include <functional>

namespace dae { class TextComponent; }

class StartMenuComponent final : public dae::Component
{
public:
	using StartGameCallback = std::function<void(GameMode)>;

	StartMenuComponent(dae::GameObject* owner, StartGameCallback startGameCallback);

	void SetTextComponents(dae::TextComponent* singlePlayerText, dae::TextComponent* coOpText, dae::TextComponent* versusText);
	void PreviousOption();
	void NextOption();
	void Confirm();

private:
	static constexpr std::size_t m_OptionCount{ 3 };

	void UpdateText();

	StartGameCallback m_StartGameCallback{};
	std::size_t m_SelectedOption{};
	bool m_Confirmed{};
	std::array<dae::TextComponent*, m_OptionCount> m_OptionTexts{};
};
