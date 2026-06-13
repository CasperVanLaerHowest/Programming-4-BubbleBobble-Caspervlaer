#include "LevelLoader.h"

#include "FruitSpawnRegistry.h"
#include "GameObjectFactory.h"
#include "../Components/LevelFlowComponent.h"
#include "../Components/PlayerSlotComponent.h"
#include "../Components/ScoreComponent.h"
#include "../Commands/SkipLevelCommand.h"

#include "GameObject.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

#include <SDL3/SDL.h>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <utility>

LevelLoader::LevelLoader(std::filesystem::path dataPath)
	: m_DataPath{ std::move(dataPath) }
{
}

dae::Scene& LevelLoader::Load(const LevelLoadSettings& settings) const
{
	UnbindGameplayControls();
	auto& scene{ dae::SceneManager::GetInstance().CreateOrClearScene("Level") };
	FruitSpawnRegistry::GetInstance().Clear();

	const auto level = LoadLevelLayout(settings.levelIndex);

	for (int row = 0; row < static_cast<int>(level.size()); ++row)
	{
		for (int col = 0; col < static_cast<int>(level[row].size()); ++col)
		{
			if (level[row][col] == '#')
			{
				glm::vec2 position{ static_cast<float>(m_OffsetX + col * m_TileSize), static_cast<float>(m_OffsetY + row * m_TileSize) };
				glm::vec2 scale{ 2, 2 };
				std::string texture{ "FakeFloor.png" };
				LoadTile(texture, position, scale, scene, CollisionType::Solid);
			}
			if (level[row][col] == '/')
			{
				glm::vec2 position{ static_cast<float>(m_OffsetX + col * m_TileSize), static_cast<float>(m_OffsetY + row * m_TileSize) };
				glm::vec2 scale{ 2, 2 };
				std::string texture{ "FakeFloor.png" };
				LoadTile(texture, position, scale, scene, CollisionType::Platform);
			}
			if (level[row][col] == 'F')
			{
				glm::vec2 position{ static_cast<float>(m_OffsetX + col * m_TileSize), static_cast<float>(m_OffsetY + row * m_TileSize) };
				FruitSpawnRegistry::GetInstance().AddSpawnPoint({ position.x, position.y - 10.f });
			}
			if (level[row][col] == 'E' && settings.gameMode != GameMode::Versus)
			{
				glm::vec2 position{ static_cast<float>(m_OffsetX + col * m_TileSize), static_cast<float>(m_OffsetY + row * m_TileSize) };
				CreateEnemy(scene, position);
			}
			if (level[row][col] == 'M' && settings.gameMode != GameMode::Versus)
			{
				glm::vec2 position{ static_cast<float>(m_OffsetX + col * m_TileSize), static_cast<float>(m_OffsetY + row * m_TileSize) };
				CreateMaitaEnemy(scene, position);
			}
		}
	}

	PlayerSettings playerOne{};
	const int connectedControllerCount = dae::InputManager::GetInstance().GetConnectedControllerCount();
	playerOne.playerIndex = 0;
	playerOne.spawnPosition = GetBottomSpawnPosition(level, false);
	playerOne.initialScore = settings.playerScores[0];
	playerOne.label = "1UP";
	playerOne.healthPosition = { 20.f, 20.f };
	playerOne.scorePosition = { 200.f, 20.f };
	playerOne.labelPosition = { 200.f, 0.f };
	playerOne.controllerIndex = settings.gameMode == GameMode::SinglePlayer || connectedControllerCount >= 2 ? 0 : -1;
	playerOne.controls = {
		SDL_SCANCODE_A,
		SDL_SCANCODE_D,
		SDL_SCANCODE_W,
		SDL_SCANCODE_S,
		SDL_SCANCODE_SPACE
	};

	CreatePlayer(scene, playerOne);

	if (settings.gameMode == GameMode::CoOp || settings.gameMode == GameMode::Versus)
	{
		PlayerSettings playerTwo{};
		playerTwo.playerIndex = 1;
		playerTwo.spawnPosition = GetBottomSpawnPosition(level, true);
		playerTwo.initialScore = settings.playerScores[1];
		playerTwo.label = settings.gameMode == GameMode::Versus ? "MAITA" : "2UP";
		playerTwo.healthPosition = { 940.f, 20.f };
		playerTwo.scorePosition = { 760.f, 20.f };
		playerTwo.labelPosition = { 760.f, 0.f };
		playerTwo.controllerIndex = connectedControllerCount >= 2 ? 1 : 0;
		playerTwo.controls = {
			SDL_SCANCODE_LEFT,
			SDL_SCANCODE_RIGHT,
			SDL_SCANCODE_UP,
			SDL_SCANCODE_DOWN,
			SDL_SCANCODE_RCTRL
		};

		if (settings.gameMode == GameMode::Versus)
		{
			playerTwo.isMaitaPlayer = true;
			playerTwo.shootsMaitaBall = true;
			playerTwo.idleFrames = { "Enemy2Move0.png", "Enemy2Move1.png", "Enemy2Move2.png", "Enemy2Move3.png", "Enemy2Move4.png" };
			playerTwo.dieFrames = { "Enemy2Angry0.png", "Enemy2Angry1.png", "Enemy2Angry2.png", "Enemy2Angry3.png", "Enemy2Angry4.png" };
			playerTwo.shootFrames = { "Enemy2Attack0.png", "Enemy2Attack1.png", "Enemy2Attack2.png", "Enemy2Attack3.png", "Enemy2Attack4.png" };
			playerTwo.walkFrames = { "Enemy2Move0.png", "Enemy2Move1.png", "Enemy2Move2.png", "Enemy2Move3.png", "Enemy2Move4.png" };
		}

		CreatePlayer(scene, playerTwo);
	}

	auto levelFlow = std::make_unique<dae::GameObject>();
	auto* levelFlowComponent = levelFlow->AddComponent<LevelFlowComponent>(settings.gameMode, settings.levelIndex);
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_F1, std::make_unique<SkipLevelCommand>(levelFlowComponent));
	scene.Add(std::move(levelFlow));

	return scene;
}

int LevelLoader::GetLevelCount() const
{
	return static_cast<int>(LoadAllLevelLayouts().size());
}

void LevelLoader::StoreCurrentPlayerScores(std::array<int, 2>& playerScores) const
{
	for (auto* collider : CollisionComponent::GetColliders())
	{
		if (collider == nullptr || collider->GetOwner()->IsDestroyed() || collider->GetCollisionType() != CollisionType::Player)
		{
			continue;
		}

		const auto* playerSlot = collider->GetOwner()->GetComponent<PlayerSlotComponent>();
		const auto* score = collider->GetOwner()->GetComponent<ScoreComponent>();
		if (playerSlot == nullptr || score == nullptr)
		{
			continue;
		}

		const int playerIndex = playerSlot->GetPlayerIndex();
		if (playerIndex >= 0 && playerIndex < static_cast<int>(playerScores.size()))
		{
			playerScores[static_cast<std::size_t>(playerIndex)] = score->GetScore();
		}
	}
}

void LevelLoader::UnbindGameplayControls() const
{
	auto& input = dae::InputManager::GetInstance();
	input.UnBindCommand(SDL_SCANCODE_A);
	input.UnBindCommand(SDL_SCANCODE_D);
	input.UnBindCommand(SDL_SCANCODE_W);
	input.UnBindCommand(SDL_SCANCODE_S);
	input.UnBindCommand(SDL_SCANCODE_SPACE);
	input.UnBindCommand(SDL_SCANCODE_LEFT);
	input.UnBindCommand(SDL_SCANCODE_RIGHT);
	input.UnBindCommand(SDL_SCANCODE_UP);
	input.UnBindCommand(SDL_SCANCODE_DOWN);
	input.UnBindCommand(SDL_SCANCODE_RCTRL);
	input.UnBindCommand(SDL_SCANCODE_RETURN);
	input.UnBindCommand(SDL_SCANCODE_F1);
	input.UnBindCommand(SDL_SCANCODE_1);
	input.UnBindCommand(SDL_SCANCODE_2);
	input.UnBindCommand(SDL_SCANCODE_3);
	input.UnBindCommand(Inputs::DPAD_UP);
	input.UnBindCommand(Inputs::DPAD_DOWN);
	input.UnBindCommand(Inputs::DPAD_LEFT);
	input.UnBindCommand(Inputs::DPAD_RIGHT);
	input.UnBindCommand(Inputs::LEFT_THUMB_UP);
	input.UnBindCommand(Inputs::LEFT_THUMB_DOWN);
	input.UnBindCommand(Inputs::LEFT_THUMB_LEFT);
	input.UnBindCommand(Inputs::LEFT_THUMB_RIGHT);
	input.UnBindCommand(Inputs::A);
	input.UnBindCommand(Inputs::X);
	input.UnBindCommand(Inputs::Y);
	input.UnBindCommand(Inputs::START);

	for (int controllerIndex = 1; controllerIndex < 4; ++controllerIndex)
	{
		input.UnBindCommand(controllerIndex, Inputs::DPAD_UP);
		input.UnBindCommand(controllerIndex, Inputs::DPAD_DOWN);
		input.UnBindCommand(controllerIndex, Inputs::DPAD_LEFT);
		input.UnBindCommand(controllerIndex, Inputs::DPAD_RIGHT);
		input.UnBindCommand(controllerIndex, Inputs::LEFT_THUMB_UP);
		input.UnBindCommand(controllerIndex, Inputs::LEFT_THUMB_DOWN);
		input.UnBindCommand(controllerIndex, Inputs::LEFT_THUMB_LEFT);
		input.UnBindCommand(controllerIndex, Inputs::LEFT_THUMB_RIGHT);
		input.UnBindCommand(controllerIndex, Inputs::A);
		input.UnBindCommand(controllerIndex, Inputs::X);
		input.UnBindCommand(controllerIndex, Inputs::Y);
		input.UnBindCommand(controllerIndex, Inputs::START);
	}
}

std::vector<std::vector<std::string>> LevelLoader::LoadAllLevelLayouts() const
{
	std::ifstream levelFile{ m_DataPath / "Levels.txt" };
	if (!levelFile.is_open())
	{
		throw std::runtime_error{ "Could not open Data/Levels.txt" };
	}

	std::vector<std::vector<std::string>> levels{};
	std::vector<std::string> currentLevel{};
	std::string line{};

	while (std::getline(levelFile, line))
	{
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		if (line.empty() || line[0] == ';')
		{
			continue;
		}

		if (line == "---")
		{
			if (!currentLevel.empty())
			{
				levels.push_back(currentLevel);
				currentLevel.clear();
			}
			continue;
		}

		currentLevel.push_back(line);
	}

	if (!currentLevel.empty())
	{
		levels.push_back(currentLevel);
	}

	return levels;
}

std::vector<std::string> LevelLoader::LoadLevelLayout(int levelIndex) const
{
	const auto levels = LoadAllLevelLayouts();
	if (levelIndex < 0 || levelIndex >= static_cast<int>(levels.size()))
	{
		throw std::runtime_error{ "Requested level does not exist in Data/Levels.txt" };
	}

	return levels[static_cast<std::size_t>(levelIndex)];
}

glm::vec2 LevelLoader::GetBottomSpawnPosition(const std::vector<std::string>& level, bool spawnRight) const
{
	const int levelHeight = static_cast<int>(level.size());
	const int levelWidth = level.empty() ? 0 : static_cast<int>(level.front().size());

	const int spawnColumn = spawnRight ? levelWidth - 3 : 1;
	const int spawnRow = levelHeight - 3;

	return {
		static_cast<float>(m_OffsetX) + static_cast<float>(spawnColumn) * m_TileSize,
		static_cast<float>(m_OffsetY) + static_cast<float>(spawnRow) * m_TileSize
	};
}

void LevelLoader::LoadTile(std::string& texture, glm::vec2& position, glm::vec2& scale, dae::Scene& scene, CollisionType type) const
{
	auto go{ std::make_unique<dae::GameObject>() };
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0);
	go->GetComponent<dae::TransformComponent>()->SetScale(scale.x, scale.y, 1);
	go->AddComponent<CollisionComponent>(
		glm::vec2{ 10, 10 },
		type,
		glm::vec2{ 0.f, 0.f }
	);
	go->AddComponent<dae::TextureComponent>()->SetTexture(texture);
	scene.Add(std::move(go));
}
