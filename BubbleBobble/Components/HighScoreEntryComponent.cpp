#include "HighScoreEntryComponent.h"

#include "TextComponent.h"
#include "SceneManager.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <utility>

HighScoreEntryComponent::HighScoreEntryComponent(dae::GameObject* owner, std::filesystem::path dataPath, int score)
	: Component(owner)
	, m_HighScoreManager{ std::make_unique<HighScoreManager>(std::move(dataPath)) }
	, m_Score{ score }
{
}

void HighScoreEntryComponent::SetTextComponents(dae::TextComponent* nameText, dae::TextComponent* statusText, dae::TextComponent* scoresText, dae::TextComponent* currentScoreText)
{
	m_NameText = nameText;
	m_StatusText = statusText;
	m_ScoresText = scoresText;
	m_CurrentScoreText = currentScoreText;

	UpdateNameText();
	if (m_StatusText)
	{
		m_StatusText->SetText("UP/DOWN CHANGE, LEFT/RIGHT SELECT, ENTER/A SAVE");
	}
}

void HighScoreEntryComponent::SetReturnToStartCallback(ReturnToStartCallback callback)
{
	m_ReturnToStart = std::move(callback);
}

void HighScoreEntryComponent::Update(float deltaTime)
{
	if (!m_Submitted || m_ReturnDelay <= 0.f)
		return;

	m_ReturnDelay -= deltaTime;
	if (m_ReturnDelay <= 0.f && m_ReturnToStart)
	{
		auto returnToStart = m_ReturnToStart;
		m_ReturnDelay = 0.f;
		dae::SceneManager::GetInstance().QueuePostUpdateAction([returnToStart]()
			{
				returnToStart();
			});
	}
}

void HighScoreEntryComponent::PreviousCharacter()
{
	if (m_Submitted)
		return;

	const int characterCount = static_cast<int>(std::size(m_Characters)) - 1;
	m_CharacterIndices[static_cast<std::size_t>(m_SelectedSlot)] = (m_CharacterIndices[static_cast<std::size_t>(m_SelectedSlot)] + characterCount - 1) % characterCount;
	UpdateNameText();
}

void HighScoreEntryComponent::NextCharacter()
{
	if (m_Submitted)
		return;

	const int characterCount = static_cast<int>(std::size(m_Characters)) - 1;
	m_CharacterIndices[static_cast<std::size_t>(m_SelectedSlot)] = (m_CharacterIndices[static_cast<std::size_t>(m_SelectedSlot)] + 1) % characterCount;
	UpdateNameText();
}

void HighScoreEntryComponent::PreviousSlot()
{
	if (m_Submitted)
		return;

	m_SelectedSlot = (m_SelectedSlot + 2) % 3;
	UpdateNameText();
}

void HighScoreEntryComponent::NextSlot()
{
	if (m_Submitted)
		return;

	m_SelectedSlot = (m_SelectedSlot + 1) % 3;
	UpdateNameText();
}

void HighScoreEntryComponent::Confirm()
{
	if (m_Submitted)
		return;

	m_Submitted = true;
	std::string name{};
	name.reserve(3);
	for (int index : m_CharacterIndices)
	{
		name.push_back(m_Characters[static_cast<std::size_t>(index)]);
	}

	const auto scores = m_HighScoreManager->AddScore({ name, m_Score });
	ShowScores(scores);

	if (m_StatusText)
	{
		m_StatusText->SetText("HIGHSCORE SAVED");
	}
	if (m_CurrentScoreText)
	{
		m_CurrentScoreText->SetText("YOUR SCORE: " + name + "  " + std::to_string(m_Score));
	}

	m_ReturnDelay = 10.f;
}

void HighScoreEntryComponent::UpdateNameText()
{
	if (m_NameText == nullptr)
		return;

	std::string text{ "NAME: " };
	for (int slot = 0; slot < 3; ++slot)
	{
		const char character = m_Characters[static_cast<std::size_t>(m_CharacterIndices[static_cast<std::size_t>(slot)])];
		if (slot == m_SelectedSlot)
		{
			text += '[';
			text += character;
			text += ']';
		}
		else
		{
			text += ' ';
			text += character;
			text += ' ';
		}
	}

	m_NameText->SetText(text);
}

void HighScoreEntryComponent::ShowScores(const std::vector<HighScoreEntry>& scores)
{
	if (m_ScoresText == nullptr)
		return;

	std::ostringstream stream{};
	stream << "TOP 10\n";
	const int scoreCount = std::min(10, static_cast<int>(scores.size()));
	for (int index = 0; index < scoreCount; ++index)
	{
		stream << index + 1 << ". " << scores[static_cast<std::size_t>(index)].name << "  " << scores[static_cast<std::size_t>(index)].score;
		if (index + 1 < scoreCount)
		{
			stream << '\n';
		}
	}

	m_ScoresText->SetText(stream.str());
}
