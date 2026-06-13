#pragma once

#include "../Components/CollisionComponent.h"
#include "../Game/GameMode.h"
#include "Scene.h"

#include <glm/ext/vector_float2.hpp>

#include <array>
#include <filesystem>
#include <string>
#include <vector>

struct LevelLoadSettings
{
	GameMode gameMode{ GameMode::SinglePlayer };
	int levelIndex{};
	std::array<int, 2> playerScores{};
};

class LevelLoader final
{
public:
	explicit LevelLoader(std::filesystem::path dataPath);

	dae::Scene& Load(const LevelLoadSettings& settings) const;
	int GetLevelCount() const;
	const std::filesystem::path& GetDataPath() const { return m_DataPath; }
	void StoreCurrentPlayerScores(std::array<int, 2>& playerScores) const;
	void UnbindGameplayControls() const;

private:
	std::vector<std::vector<std::string>> LoadAllLevelLayouts() const;
	std::vector<std::string> LoadLevelLayout(int levelIndex) const;
	glm::vec2 GetBottomSpawnPosition(const std::vector<std::string>& level, bool spawnRight) const;
	void LoadTile(std::string& texture, glm::vec2& position, glm::vec2& scale, dae::Scene& scene, CollisionType type) const;

	std::filesystem::path m_DataPath{};
	float m_TileSize{ 23.f };
	int m_OffsetX{ 200 };
	int m_OffsetY{ 40 };
};
