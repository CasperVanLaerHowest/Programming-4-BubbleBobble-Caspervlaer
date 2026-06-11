#pragma once
#include "Scene.h"
#include <SDL3/SDL.h>
#include <glm/ext/vector_float2.hpp>
#include <string>

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
	glm::vec2 spawnPosition{ 400.f, 300.f };
	glm::vec2 scale{ 2.f, 2.f };
	int health{ 3 };
	std::string label{ "1UP" };
	glm::vec2 healthPosition{ 20.f, 20.f };
	glm::vec2 scorePosition{ 200.f, 20.f };
	glm::vec2 labelPosition{ 200.f, 0.f };
	PlayerControls controls{};
};

void CreatePlayer(dae::Scene& scene, const PlayerSettings& settings);

void SpawnBubble(dae::Scene& scene, const glm::vec2& position, bool facingRight);
