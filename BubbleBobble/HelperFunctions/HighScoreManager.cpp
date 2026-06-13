#include "HighScoreManager.h"

#include <algorithm>
#include <fstream>
#include <utility>

HighScoreManager::HighScoreManager(std::filesystem::path dataPath)
	: m_DataPath{ std::move(dataPath) }
{
}

std::vector<HighScoreEntry> HighScoreManager::LoadScores() const
{
	std::ifstream file{ GetScorePath() };
	std::vector<HighScoreEntry> scores{};

	std::string name{};
	int score{};
	while (file >> name >> score)
	{
		if (name.size() > 3)
		{
			name = name.substr(0, 3);
		}

		scores.push_back({ name, score });
	}

	std::sort(scores.begin(), scores.end(), [](const HighScoreEntry& lhs, const HighScoreEntry& rhs)
		{
			return lhs.score > rhs.score;
		});

	return scores;
}

std::vector<HighScoreEntry> HighScoreManager::AddScore(const HighScoreEntry& entry) const
{
	auto scores = LoadScores();
	scores.push_back(entry);

	std::sort(scores.begin(), scores.end(), [](const HighScoreEntry& lhs, const HighScoreEntry& rhs)
		{
			return lhs.score > rhs.score;
		});

	std::ofstream file{ GetScorePath(), std::ios::trunc };
	for (const auto& score : scores)
	{
		file << score.name << ' ' << score.score << '\n';
	}

	return scores;
}

std::filesystem::path HighScoreManager::GetScorePath() const
{
	return m_DataPath / "HighScores.txt";
}
