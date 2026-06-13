#pragma once

#include <filesystem>
#include <string>
#include <vector>

struct HighScoreEntry
{
	std::string name{};
	int score{};
};

class HighScoreManager final
{
public:
	explicit HighScoreManager(std::filesystem::path dataPath);

	std::vector<HighScoreEntry> LoadScores() const;
	std::vector<HighScoreEntry> AddScore(const HighScoreEntry& entry) const;

private:
	std::filesystem::path GetScorePath() const;

	std::filesystem::path m_DataPath{};
};
