#include "HelperFunctions.h"
#include "Components/CollisionComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/PlayerStateComponent.h"
#include "Components/PlayerFacingComponent.h"
#include "Components/PhysicsComponent.h"
#include "Commands/MoveCommand.h"
#include "Commands/JumpCommand.h"

#include <TransformComponent.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include "Commands/ShootBubbleCommand.h"


void CreatePlayer(dae::Scene& scene) {
	auto player{ std::make_unique<dae::GameObject>() };
	player->GetComponent<dae::TransformComponent>()->SetLocalPosition(400, 300, 0);
	player->GetComponent<dae::TransformComponent>()->SetScale(2, 2, 1);
	player->AddComponent<PhysicsComponent>();
	player->AddComponent<CollisionComponent>(glm::vec2{ 20, 20 }, CollisionType::Player, glm::vec2{ 0.f, 0.f });
	player->AddComponent<PlayerFacingComponent>();

	std::vector<std::shared_ptr<dae::Texture2D>> idleFrames = {
	dae::ResourceManager::GetInstance().LoadTexture("PlayerIdle0.png"),
	dae::ResourceManager::GetInstance().LoadTexture("PlayerIdle1.png")
	};

	std::vector<std::shared_ptr<dae::Texture2D>> dieFrames = {
	dae::ResourceManager::GetInstance().LoadTexture("PlayerDie0.png"),
	dae::ResourceManager::GetInstance().LoadTexture("PlayerDie1.png"),
	dae::ResourceManager::GetInstance().LoadTexture("PlayerDie2.png")
	};

	std::vector<std::shared_ptr<dae::Texture2D>> shootFrames = {
	dae::ResourceManager::GetInstance().LoadTexture("PlayerShoot0.png"),
	dae::ResourceManager::GetInstance().LoadTexture("PlayerShoot1.png"),
	dae::ResourceManager::GetInstance().LoadTexture("PlayerShoot2.png"),
	dae::ResourceManager::GetInstance().LoadTexture("PlayerShoot3.png")
	};

	std::vector<std::shared_ptr<dae::Texture2D>> walkFrames = {
	dae::ResourceManager::GetInstance().LoadTexture("PlayerWalk0.png"),
	dae::ResourceManager::GetInstance().LoadTexture("PlayerWalk2.png"),
	dae::ResourceManager::GetInstance().LoadTexture("PlayerWalk1.png"),
	dae::ResourceManager::GetInstance().LoadTexture("PlayerWalk3.png")
	};

	auto animationComp = player->AddComponent<AnimationComponent>();
	animationComp->AddAnimation("Idle", idleFrames, 0.5f, true);
	animationComp->AddAnimation("Die", dieFrames, 0.3f, false);
	animationComp->AddAnimation("Shoot", shootFrames, 0.2f, false);
	animationComp->AddAnimation("Walk", walkFrames, 0.2f, true);

	player->AddComponent<PlayerStateComponent>();
	//player->AddComponent<HealthComponent>(3)->AddObserver(healthText->GetComponent<HealthObserver>());
	//player->AddComponent<ScoreComponent>(0)->AddObserver(scoreText->GetComponent<ScoreObserver>());
	//player->GetComponent<ScoreComponent>()->AddObserver(scoreText->GetComponent<SteamWinObserver>());


	auto cmdjump = std::make_unique<JumpCommand>(player.get());
	cmdjump->SetJumpStrength(400.f);

	auto cmdshoot = std::make_unique<ShootBubbleCommand>(player.get());

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, std::make_unique<MoveCommand>(player.get(), glm::vec2{ -1, 0 }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 1, 0 }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, std::move(cmdjump));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 0, 1 }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_SPACE, std::move(cmdshoot));
	scene.Add(std::move(player));
}
