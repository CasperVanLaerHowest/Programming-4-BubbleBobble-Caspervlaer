#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "InputManager.h"

#include "Components/TextComponent.h"
#include "Components/FPSComponent.h"
#include "Components/RotatorComponent.h"
#include "Components/ImGuiComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/PlayerStateComponent.h"

#include "Commands/MoveCommand.h"
#include "Commands/TakeDamageCommand.h"
#include "Commands/AddScoreCommand.h"
#include "Commands/JumpCommand.h"
#include "Commands/StartGameCommand.h"

#include "Observers/HealthObserver.h"
#include "Observers/ScoreObserver.h"

#include "HelperFunctions/GameObjectFactory.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

void StartGame(GameMode gameMode);
bool g_GameStarted{};
std::filesystem::path g_DataPath{};

std::vector<std::string> LoadLevelLayout(int levelIndex)
{
	std::ifstream levelFile{ g_DataPath / "Levels.txt" };
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

	if (levelIndex < 0 || levelIndex >= static_cast<int>(levels.size()))
	{
		throw std::runtime_error{ "Requested level does not exist in Data/Levels.txt" };
	}

	return levels[static_cast<std::size_t>(levelIndex)];
}

void AddText(dae::Scene& scene, const std::string& text, const glm::vec2& position, uint8_t fontSize)
{
	auto go = std::make_unique<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0.f);
	go->AddComponent<dae::TextComponent>(text, font);
	scene.Add(std::move(go));
}

void TileLoader(std::string& texture, glm::vec2& position, glm::vec2& scale, dae::Scene& scene, CollisionType type) {
	auto go {std::make_unique<dae::GameObject>()};
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

dae::Scene& LevelLoad(GameMode gameMode) {
	auto& scene {dae::SceneManager::GetInstance().CreateScene("Level")};
	ClearFruitSpawnPoints();

	constexpr float tileSize {23.f};
	constexpr int offsetX {200};
	constexpr int offsetY {40};

	const auto level = LoadLevelLayout(0);

	for (int row = 0; row < static_cast<int>(level.size()); ++row)
	{
		for (int col = 0; col < static_cast<int>(level[row].size()); ++col)
		{
			if (level[row][col] == '#')
			{
				glm::vec2 position{ static_cast<float>(offsetX + col * tileSize), static_cast<float>(offsetY + row * tileSize) };
				glm::vec2 scale{ 2, 2 };
				std::string texture {"FakeFloor.png"};
				TileLoader(texture, position, scale, scene, CollisionType::Solid);
			}
			if (level[row][col] == '/')
			{
				glm::vec2 position{ static_cast<float>(offsetX + col * tileSize), static_cast<float>(offsetY + row * tileSize) };
				glm::vec2 scale{ 2, 2 };
				std::string texture{ "FakeFloor.png" };
				TileLoader(texture, position, scale, scene, CollisionType::Platform);
			}
			if (level[row][col] == 'F')
			{
				glm::vec2 position{ static_cast<float>(offsetX + col * tileSize), static_cast<float>(offsetY + row * tileSize) };
				AddFruitSpawnPoint({ position.x, position.y - 10.f });
			}
		}
	}

	if (gameMode != GameMode::Versus)
	{
		CreateEnemy(scene, { 500.f, 300.f });
	}

	PlayerSettings playerOne{};
	playerOne.spawnPosition = { 400.f, 300.f };
	playerOne.label = "1UP";
	playerOne.healthPosition = { 20.f, 20.f };
	playerOne.scorePosition = { 200.f, 20.f };
	playerOne.labelPosition = { 200.f, 0.f };
	playerOne.controls = {
		SDL_SCANCODE_A,
		SDL_SCANCODE_D,
		SDL_SCANCODE_W,
		SDL_SCANCODE_S,
		SDL_SCANCODE_SPACE
	};

	CreatePlayer(scene, playerOne);

	if (gameMode == GameMode::CoOp || gameMode == GameMode::Versus)
	{
		PlayerSettings playerTwo{};
		playerTwo.spawnPosition = { 450.f, 300.f };
		playerTwo.label = gameMode == GameMode::Versus ? "MAITA" : "2UP";
		playerTwo.healthPosition = { 940.f, 20.f };
		playerTwo.scorePosition = { 760.f, 20.f };
		playerTwo.labelPosition = { 760.f, 0.f };
		playerTwo.controls = {
			SDL_SCANCODE_LEFT,
			SDL_SCANCODE_RIGHT,
			SDL_SCANCODE_UP,
			SDL_SCANCODE_DOWN,
			SDL_SCANCODE_RCTRL
		};

		if (gameMode == GameMode::Versus)
		{
			playerTwo.idleFrames = { "EnemyMove0.png", "EnemyMove1.png" };
			playerTwo.dieFrames = { "EnemyAngry0.png", "EnemyAngry1.png", "EnemyAngry2.png", "EnemyAngry3.png" };
			playerTwo.shootFrames = { "EnemyAngry0.png", "EnemyAngry1.png", "EnemyAngry2.png", "EnemyAngry3.png" };
			playerTwo.walkFrames = { "EnemyMove0.png", "EnemyMove1.png", "EnemyMove2.png", "EnemyMove3.png" };
		}

		CreatePlayer(scene, playerTwo);
	}

	return scene;
}

dae::Scene& StartScreenLoad()
{
	auto& scene{ dae::SceneManager::GetInstance().CreateScene("Start") };

	AddText(scene, "BUBBLE BOBBLE", { 260.f, 120.f }, 36);
	AddText(scene, "1 SINGLE PLAYER", { 300.f, 240.f }, 24);
	AddText(scene, "2 CO-OP", { 300.f, 280.f }, 24);
	AddText(scene, "3 VERSUS", { 300.f, 320.f }, 24);
	AddText(scene, "PUSH 1, 2 OR 3", { 300.f, 380.f }, 18);

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_1, std::make_unique<StartGameCommand>(GameMode::SinglePlayer, StartGame));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_2, std::make_unique<StartGameCommand>(GameMode::CoOp, StartGame));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_3, std::make_unique<StartGameCommand>(GameMode::Versus, StartGame));

	return scene;
}

void StartGame(GameMode gameMode)
{
	if (g_GameStarted)
		return;

	g_GameStarted = true;

	LevelLoad(gameMode);
	dae::SceneManager::GetInstance().SetActiveScene("Level");
}

static void load()
{
	StartScreenLoad();
	dae::SceneManager::GetInstance().SetActiveScene("Start");
	return;

#if 0
	/*
	//auto& scene = dae::SceneManager::GetInstance().CreateScene();

	//auto go = std::make_unique<dae::GameObject>();
	//go->AddComponent<dae::TextureComponent>()->SetTexture("background.png");
	//scene.Add(std::move(go));

	//go = std::make_unique<dae::GameObject>();
	//go->AddComponent<dae::TextureComponent>()->SetTexture("logo.png");
	//go->GetComponent<dae::TransformComponent>()->SetLocalPosition(358, 180,0);
	//scene.Add(std::move(go));

	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	//go = std::make_unique<dae::GameObject>();
	//go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	//go->GetComponent<dae::TransformComponent>()->SetLocalPosition(292, 20, 0);
	//scene.Add(std::move(go));

	//go = std::make_unique<dae::GameObject>();
	//go->AddComponent<dae::TextComponent>("FPS: ", font);
	//go->AddComponent<dae::FPSComponent>();
	//go->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 20, 0);
	//scene.Add(std::move(go));

	//auto healthText = std::make_unique<dae::GameObject>();
	//healthText->AddComponent<dae::TextComponent>("Health: 3", font2);
	//healthText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 120, 0);
	//healthText->AddComponent<HealthObserver>();

	//auto scoreText = std::make_unique<dae::GameObject>();
	//scoreText->AddComponent<dae::TextComponent>("Score: 0", font2);
	//scoreText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 150, 0);
	//scoreText->AddComponent<ScoreObserver>();
	//scoreText->AddComponent<SteamWinObserver>();
	//
	//auto controllsText = std::make_unique<dae::GameObject>();
	//controllsText->AddComponent<dae::TextComponent>("Use the WASD keys to move, K to take damage, Z to score points", font2);
	//controllsText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 60, 0);
	//scene.Add(std::move(controllsText));







	//go = std::make_unique<dae::GameObject>();
	//go->GetComponent<dae::TransformComponent>()->SetLocalPosition(400, 300, 0);
	//go->GetComponent<dae::TransformComponent>()->SetScale(2, 2, 1);
	//go->AddComponent<PhysicsComponent>();
	//go->AddComponent<CollisionComponent>(glm::vec2{ 20, 20 }, glm::vec2{ 0.f, 0.f });
	////go->AddComponent<dae::TextureComponent>()->SetTexture("Sprite1.png");
	//

	//std::vector<std::shared_ptr<dae::Texture2D>> idleFrames = {
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerIdle0.png"),
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerIdle1.png")
	//};

	//std::vector<std::shared_ptr<dae::Texture2D>> dieFrames = {
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerDie0.png"),
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerDie1.png"),
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerDie2.png")
	//};

	//std::vector<std::shared_ptr<dae::Texture2D>> shootFrames = {
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerShoot0.png"),
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerShoot1.png"),
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerShoot2.png"),
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerShoot3.png")
	//};

	//std::vector<std::shared_ptr<dae::Texture2D>> walkFrames = {
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerWalk0.png"),
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerWalk2.png"),
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerWalk1.png"),
	//dae::ResourceManager::GetInstance().LoadTexture("PlayerWalk3.png")
	//};

	//auto animationComp = go->AddComponent<AnimationComponent>();
	//animationComp->AddAnimation("Idle", idleFrames, 0.5f, true);
	//animationComp->AddAnimation("Die", dieFrames, 0.3f, false);
	//animationComp->AddAnimation("Shoot", shootFrames, 0.2f, false);
	//animationComp->AddAnimation("Walk", walkFrames, 0.2f, true);

	//go->AddComponent<PlayerStateComponent>();
	//go->AddComponent<HealthComponent>(3)->AddObserver(healthText->GetComponent<HealthObserver>());
	//go->AddComponent<ScoreComponent>(0)->AddObserver(scoreText->GetComponent<ScoreObserver>());
	//go->GetComponent<ScoreComponent>()->AddObserver(scoreText->GetComponent<SteamWinObserver>());

	//auto cmdjump = std::make_unique<JumpCommand>(go.get());
	//cmdjump->SetJumpStrength(400.f);

	//dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, std::make_unique<MoveCommand>(go.get(), glm::vec2{-1, 0}));
	//dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, std::make_unique<MoveCommand>(go.get(), glm::vec2{1, 0}));
	//dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, std::move(cmdjump));
	//dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, std::make_unique<MoveCommand>(go.get(), glm::vec2{0, 1}));

	//dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_K, std::make_unique<TakeDamageCommand>(go.get()));
	//dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_Z, std::make_unique<AddScoreCommand>(go.get()));
	//scene.Add(std::move(go));
	//scene.Add(std::move(healthText));
	//scene.Add(std::move(scoreText));


	//healthText = std::make_unique<dae::GameObject>();
	//healthText->AddComponent<dae::TextComponent>("Health: 3", font2);
	//healthText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 190, 0);
	//healthText->AddComponent<HealthObserver>();

	//scoreText = std::make_unique<dae::GameObject>();
	//scoreText->AddComponent<dae::TextComponent>("Score: 0", font2);
	//scoreText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 220, 0);
	//scoreText->AddComponent<ScoreObserver>();
	//scoreText->AddComponent<SteamWinObserver>();

	//controllsText = std::make_unique<dae::GameObject>();
	//controllsText->AddComponent<dae::TextComponent>("Use the D-Pad to move, A to take damage, B to score points", font2);
	//controllsText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 80, 0);
	//scene.Add(std::move(controllsText));

	//go = std::make_unique<dae::GameObject>();
	//go->GetComponent<dae::TransformComponent>()->SetLocalPosition(450, 300, 0);
	//go->AddComponent<dae::TextureComponent>()->SetTexture("Sprite2.png");
	//go->AddComponent<HealthComponent>(3)->AddObserver(healthText->GetComponent<HealthObserver>());
	//go->AddComponent<ScoreComponent>(0)->AddObserver(scoreText->GetComponent<ScoreObserver>());
	//go->GetComponent<ScoreComponent>()->AddObserver(scoreText->GetComponent<SteamWinObserver>());

	//auto cmdLeft = std::make_unique<MoveCommand>(go.get(), glm::vec2{-1, 0});
	//cmdLeft->SetSpeed(200.f);
	//dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_LEFT, std::move(cmdLeft));

	//auto cmdRight = std::make_unique<MoveCommand>(go.get(), glm::vec2{1, 0});
	//cmdRight->SetSpeed(200.f);
	//dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_RIGHT, std::move(cmdRight));

	//auto cmdUp = std::make_unique<MoveCommand>(go.get(), glm::vec2{0, -1});
	//cmdUp->SetSpeed(200.f);
	//dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_UP, std::move(cmdUp));

	//auto cmdDown = std::make_unique<MoveCommand>(go.get(), glm::vec2{0, 1});
	//cmdDown->SetSpeed(200.f);
	//dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_DOWN, std::move(cmdDown));

	//dae::InputManager::GetInstance().BindCommand(Inputs::A, std::make_unique<TakeDamageCommand>(go.get()));
	//dae::InputManager::GetInstance().BindCommand(Inputs::B, std::make_unique<AddScoreCommand>(go.get()));
	//scene.Add(std::move(go));
	//scene.Add(std::move(healthText));
	//scene.Add(std::move(scoreText));
	*/

	auto& scene = LevelLoad(GameMode::SinglePlayer);
	
	PlayerSettings playerOne{};
	playerOne.spawnPosition = { 400.f, 300.f };
	playerOne.label = "1UP";
	playerOne.healthPosition = { 20.f, 20.f };
	playerOne.scorePosition = { 200.f, 20.f };
	playerOne.labelPosition = { 200.f, 0.f };
	playerOne.controls = {
		SDL_SCANCODE_A,
		SDL_SCANCODE_D,
		SDL_SCANCODE_W,
		SDL_SCANCODE_S,
		SDL_SCANCODE_SPACE
	};

	CreateEnemy(scene, { 500.f, 300.f });

	CreatePlayer(scene, playerOne);
#endif
}



int main(int, char*[]) {
#if __EMSCRIPTEN__
	std::filesystem::path data_location = "";
#else
	std::filesystem::path data_location = "./Data/";
	if(!std::filesystem::exists(data_location))
		data_location = "../Data/";
#endif
	g_DataPath = data_location;
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
