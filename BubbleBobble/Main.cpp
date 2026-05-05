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

#include "Commands/MoveCommand.h"
#include "Commands/TakeDamageCommand.h"
#include "Commands/AddScoreCommand.h"

#include "Observers/HealthObserver.h"
#include "Observers/ScoreObserver.h"
#include "Observers/SteamWinObserver.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>()->SetTexture("background.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>()->SetTexture("logo.png");
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(358, 180,0);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(292, 20, 0);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("FPS: ", font);
	go->AddComponent<dae::FPSComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 20, 0);
	scene.Add(std::move(go));

	auto healthText = std::make_unique<dae::GameObject>();
	healthText->AddComponent<dae::TextComponent>("Health: 3", font2);
	healthText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 120, 0);
	healthText->AddComponent<HealthObserver>();

	auto scoreText = std::make_unique<dae::GameObject>();
	scoreText->AddComponent<dae::TextComponent>("Score: 0", font2);
	scoreText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 150, 0);
	scoreText->AddComponent<ScoreObserver>();
	scoreText->AddComponent<SteamWinObserver>();
	
	auto controllsText = std::make_unique<dae::GameObject>();
	controllsText->AddComponent<dae::TextComponent>("Use the WASD keys to move, K to take damage, Z to score points", font2);
	controllsText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 60, 0);
	scene.Add(std::move(controllsText));

	go = std::make_unique<dae::GameObject>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(400, 300, 0);
	go->AddComponent<PhysicsComponent>();
	go->AddComponent<CollisionComponent>(glm::vec2{ 20, 20 }, glm::vec2{ -10, -10 });
	go->AddComponent<dae::TextureComponent>()->SetTexture("Sprite1.png");
	go->AddComponent<HealthComponent>(3)->AddObserver(healthText->GetComponent<HealthObserver>());
	go->AddComponent<ScoreComponent>(0)->AddObserver(scoreText->GetComponent<ScoreObserver>());
	go->GetComponent<ScoreComponent>()->AddObserver(scoreText->GetComponent<SteamWinObserver>());

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, std::make_unique<MoveCommand>(go.get(), glm::vec2{-1, 0}));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, std::make_unique<MoveCommand>(go.get(), glm::vec2{1, 0}));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, std::make_unique<MoveCommand>(go.get(), glm::vec2{0, -1}));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, std::make_unique<MoveCommand>(go.get(), glm::vec2{0, 1}));

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_K, std::make_unique<TakeDamageCommand>(go.get()));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_Z, std::make_unique<AddScoreCommand>(go.get()));
	scene.Add(std::move(go));
	scene.Add(std::move(healthText));
	scene.Add(std::move(scoreText));

	go = std::make_unique<dae::GameObject>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(400, 400, 0);
	go->AddComponent<CollisionComponent>(glm::vec2{ 10, 10 }, glm::vec2{ -5, -5 });
	go->AddComponent<dae::TextureComponent>()->SetTexture("FakeFloor.png");

	scene.Add(std::move(go));

	healthText = std::make_unique<dae::GameObject>();
	healthText->AddComponent<dae::TextComponent>("Health: 3", font2);
	healthText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 190, 0);
	healthText->AddComponent<HealthObserver>();

	scoreText = std::make_unique<dae::GameObject>();
	scoreText->AddComponent<dae::TextComponent>("Score: 0", font2);
	scoreText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 220, 0);
	scoreText->AddComponent<ScoreObserver>();
	scoreText->AddComponent<SteamWinObserver>();

	controllsText = std::make_unique<dae::GameObject>();
	controllsText->AddComponent<dae::TextComponent>("Use the D-Pad to move, A to take damage, B to score points", font2);
	controllsText->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 80, 0);
	scene.Add(std::move(controllsText));

	go = std::make_unique<dae::GameObject>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(450, 300, 0);
	go->AddComponent<dae::TextureComponent>()->SetTexture("Sprite2.png");
	go->AddComponent<HealthComponent>(3)->AddObserver(healthText->GetComponent<HealthObserver>());
	go->AddComponent<ScoreComponent>(0)->AddObserver(scoreText->GetComponent<ScoreObserver>());
	go->GetComponent<ScoreComponent>()->AddObserver(scoreText->GetComponent<SteamWinObserver>());

	auto cmdLeft = std::make_unique<MoveCommand>(go.get(), glm::vec2{-1, 0});
	cmdLeft->SetSpeed(200.f);
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_LEFT, std::move(cmdLeft));

	auto cmdRight = std::make_unique<MoveCommand>(go.get(), glm::vec2{1, 0});
	cmdRight->SetSpeed(200.f);
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_RIGHT, std::move(cmdRight));

	auto cmdUp = std::make_unique<MoveCommand>(go.get(), glm::vec2{0, -1});
	cmdUp->SetSpeed(200.f);
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_UP, std::move(cmdUp));

	auto cmdDown = std::make_unique<MoveCommand>(go.get(), glm::vec2{0, 1});
	cmdDown->SetSpeed(200.f);
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_DOWN, std::move(cmdDown));

	dae::InputManager::GetInstance().BindCommand(Inputs::A, std::make_unique<TakeDamageCommand>(go.get()));
	dae::InputManager::GetInstance().BindCommand(Inputs::B, std::make_unique<AddScoreCommand>(go.get()));
	scene.Add(std::move(go));
	scene.Add(std::move(healthText));
	scene.Add(std::move(scoreText));
	
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
