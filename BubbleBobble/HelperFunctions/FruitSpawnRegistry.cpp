#include "FruitSpawnRegistry.h"

#include <limits>

FruitSpawnRegistry& FruitSpawnRegistry::GetInstance()
{
	static FruitSpawnRegistry instance{};
	return instance;
}

void FruitSpawnRegistry::Clear()
{
	m_SpawnPoints.clear();
}

void FruitSpawnRegistry::AddSpawnPoint(const glm::vec2& position)
{
	m_SpawnPoints.push_back(position);
}

glm::vec2 FruitSpawnRegistry::GetClosestSpawnPoint(const glm::vec2& position) const
{
	if (m_SpawnPoints.empty())
		return position;

	glm::vec2 closestPoint = m_SpawnPoints.front();
	float closestDistanceSquared = (std::numeric_limits<float>::max)();

	for (const auto& point : m_SpawnPoints)
	{
		const glm::vec2 difference = point - position;
		const float distanceSquared = difference.x * difference.x + difference.y * difference.y;
		if (distanceSquared < closestDistanceSquared)
		{
			closestDistanceSquared = distanceSquared;
			closestPoint = point;
		}
	}

	return closestPoint;
}
