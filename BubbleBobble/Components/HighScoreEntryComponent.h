#pragma once

#include "../HelperFunctions/HighScoreManager.h"
#include "Component.h"

#include <array>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dae { class TextComponent; }

class HighScoreEntryComponent final : public dae::Component
{
public:
	using ReturnToStartCallback = std::function<void()>;

	HighScoreEntryComponent(dae::GameObject* owner, std::filesystem::path dataPath, int score);

	void SetTextComponents(dae::TextComponent* nameText, dae::TextComponent* statusText, dae::TextComponent* scoresText, dae::TextComponent* currentScoreText);
	void SetReturnToStartCallback(ReturnToStartCallback callback);
	void Update(float deltaTime) override;

	void PreviousCharacter();
	void NextCharacter();
	void PreviousSlot();
	void NextSlot();
	void Confirm();

private:
	void UpdateNameText();
	void ShowScores(const std::vector<HighScoreEntry>& scores);

	std::unique_ptr<HighScoreManager> m_HighScoreManager{};
	int m_Score{};
	std::array<int, 3> m_CharacterIndices{};
	int m_SelectedSlot{};
	bool m_Submitted{};
	float m_ReturnDelay{};
	ReturnToStartCallback m_ReturnToStart{};

	dae::TextComponent* m_NameText{};
	dae::TextComponent* m_StatusText{};
	dae::TextComponent* m_ScoresText{};
	dae::TextComponent* m_CurrentScoreText{};

	inline static constexpr char m_Characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
};
