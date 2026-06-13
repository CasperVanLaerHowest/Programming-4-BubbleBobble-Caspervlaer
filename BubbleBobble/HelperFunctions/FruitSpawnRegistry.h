#pragma once

#include <glm/ext/vector_float2.hpp>

#include <vector>

class FruitSpawnRegistry final
{
public:
	static FruitSpawnRegistry& GetInstance();

	void Clear();
	void AddSpawnPoint(const glm::vec2& position);
	glm::vec2 GetClosestSpawnPoint(const glm::vec2& position) const;

private:
	FruitSpawnRegistry() = default;

	std::vector<glm::vec2> m_SpawnPoints{};
};
