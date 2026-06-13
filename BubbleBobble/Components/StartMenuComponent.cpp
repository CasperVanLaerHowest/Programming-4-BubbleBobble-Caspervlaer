#include "StartMenuComponent.h"

#include "SceneManager.h"
#include "TextComponent.h"

#include <string>
#include <utility>

namespace
{
	constexpr std::array<GameMode, 3> gameModes{
		GameMode::SinglePlayer,
		GameMode::CoOp,
		GameMode::Versus
	};

	constexpr std::array<const char*, 3> labels{
		"SINGLE PLAYER",
		"CO-OP",
		"VERSUS"
	};
}

StartMenuComponent::StartMenuComponent(dae::GameObject* owner, StartGameCallback startGameCallback)
	: Component(owner)
	, m_StartGameCallback{ std::move(startGameCallback) }
{
}

void StartMenuComponent::SetTextComponents(dae::TextComponent* singlePlayerText, dae::TextComponent* coOpText, dae::TextComponent* versusText)
{
	m_OptionTexts = { singlePlayerText, coOpText, versusText };
	UpdateText();
}

void StartMenuComponent::PreviousOption()
{
	m_SelectedOption = (m_SelectedOption + m_OptionCount - 1) % m_OptionCount;
	UpdateText();
}

void StartMenuComponent::NextOption()
{
	m_SelectedOption = (m_SelectedOption + 1) % m_OptionCount;
	UpdateText();
}

void StartMenuComponent::Confirm()
{
	if (m_Confirmed || !m_StartGameCallback)
		return;

	m_Confirmed = true;

	const auto startGameCallback = m_StartGameCallback;
	const GameMode selectedGameMode = gameModes[m_SelectedOption];
	dae::SceneManager::GetInstance().QueuePostUpdateAction([startGameCallback, selectedGameMode]()
		{
			startGameCallback(selectedGameMode);
		});
}

void StartMenuComponent::UpdateText()
{
	for (std::size_t index = 0; index < m_OptionTexts.size(); ++index)
	{
		if (m_OptionTexts[index] == nullptr)
			continue;

		const bool isSelected = index == m_SelectedOption;
		m_OptionTexts[index]->SetText(isSelected
			? "[" + std::string{ labels[index] } + "]"
			: " " + std::string{ labels[index] } + " ");
	}
}
