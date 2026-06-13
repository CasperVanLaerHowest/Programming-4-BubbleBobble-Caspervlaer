#pragma once
#include "Scene.h"
#include <SDL3/SDL.h>
#include <glm/ext/vector_float2.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dae
{
	class GameObject;
	class TextComponent;
}

struct PlayerControls
{
	SDL_Scancode left{ SDL_SCANCODE_A };
	SDL_Scancode right{ SDL_SCANCODE_D };
	SDL_Scancode jump{ SDL_SCANCODE_W };
	SDL_Scancode down{ SDL_SCANCODE_S };
	SDL_Scancode shoot{ SDL_SCANCODE_SPACE };
};

struct PlayerSettings
{
	int playerIndex{};
	glm::vec2 spawnPosition{ 400.f, 300.f };
	glm::vec2 scale{ 2.f, 2.f };
	int health{ 4 };
	int initialScore{};
	std::string label{ "1UP" };
	glm::vec2 healthPosition{ 20.f, 20.f };
	glm::vec2 scorePosition{ 200.f, 20.f };
	glm::vec2 labelPosition{ 200.f, 0.f };
	bool shootsMaitaBall{};
	bool isMaitaPlayer{};
	int controllerIndex{ -1 };
	PlayerControls controls{};
	std::vector<std::string> idleFrames{ "PlayerIdle0.png", "PlayerIdle1.png" };
	std::vector<std::string> dieFrames{ "PlayerDie0.png", "PlayerDie1.png", "PlayerDie2.png" };
	std::vector<std::string> shootFrames{ "PlayerShoot0.png", "PlayerShoot1.png", "PlayerShoot2.png", "PlayerShoot3.png" };
	std::vector<std::string> walkFrames{ "PlayerWalk0.png", "PlayerWalk2.png", "PlayerWalk1.png", "PlayerWalk3.png" };
};

void CreatePlayer(dae::Scene& scene, const PlayerSettings& settings);

void CreateEnemy(dae::Scene& scene, const glm::vec2& position);
void CreateMaitaEnemy(dae::Scene& scene, const glm::vec2& position);

void AddText(dae::Scene& scene, const std::string& text, const glm::vec2& position, uint8_t fontSize);

std::unique_ptr<dae::GameObject> CreateTextObject(const std::string& text, const glm::vec2& position, uint8_t fontSize, dae::TextComponent** textComponent = nullptr);

void SpawnBubble(dae::Scene& scene, const glm::vec2& position, bool facingRight);
void SpawnMaitaBall(dae::Scene& scene, const glm::vec2& position, float direction, int ownerPlayerIndex = -1);

void SpawnFruit(dae::Scene& scene, const glm::vec2& position);
void SpawnFruit(dae::Scene& scene, const glm::vec2& position, const std::string& texture, int scoreValue);
