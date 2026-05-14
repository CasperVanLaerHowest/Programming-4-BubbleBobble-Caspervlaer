#include "GameObjectFactory.h"
#include "../Components/CollisionComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/PlayerStateComponent.h"
#include "../Components/FacingComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Commands/MoveCommand.h"
#include "../Commands/JumpCommand.h"

#include <TransformComponent.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include "../Commands/ShootBubbleCommand.h"
#include "../Components/BubbleStateComponent.h"



void CreatePlayer(dae::Scene& scene) {
	auto player{ std::make_unique<dae::GameObject>() };
	player->GetComponent<dae::TransformComponent>()->SetLocalPosition(400, 300, 0);
	player->GetComponent<dae::TransformComponent>()->SetScale(2, 2, 1);
	player->AddComponent<PhysicsComponent>();
	player->AddComponent<CollisionComponent>(glm::vec2{ 20, 20 }, CollisionType::Player, glm::vec2{ 0.f, 0.f });
	player->AddComponent<FacingComponent>();

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

	auto cmdshoot = std::make_unique<ShootBubbleCommand>(player.get(), scene);

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, std::make_unique<MoveCommand>(player.get(), glm::vec2{ -1, 0 }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 1, 0 }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, std::move(cmdjump));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 0, 1 }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_SPACE, std::move(cmdshoot));
	scene.Add(std::move(player));
}

void SpawnBubble(dae::Scene& scene, const glm::vec2& spawnPos, bool facingRight) {
	auto bubble = std::make_unique<dae::GameObject>();
	bubble->GetComponent<dae::TransformComponent>()->SetLocalPosition(spawnPos.x, spawnPos.y, 0.f);
	bubble->GetComponent<dae::TransformComponent>()->SetScale(2.f, 2.f, 1.f);

	std::vector<std::shared_ptr<dae::Texture2D>> BubbleFrames = {
	dae::ResourceManager::GetInstance().LoadTexture("Bubble6.png"),
	dae::ResourceManager::GetInstance().LoadTexture("Bubble5.png"),
	dae::ResourceManager::GetInstance().LoadTexture("Bubble4.png"),
	dae::ResourceManager::GetInstance().LoadTexture("Bubble3.png"),
	dae::ResourceManager::GetInstance().LoadTexture("Bubble2.png"),
	dae::ResourceManager::GetInstance().LoadTexture("Bubble1.png")
	};

	bubble->AddComponent<AnimationComponent>()->AddAnimation("Bubble", BubbleFrames, 0.1f, false);
	bubble->GetComponent<AnimationComponent>()->PlayAnimation("Bubble");
	bubble->AddComponent<FacingComponent>()->SetFacingDirection(facingRight ? FacingDirection::Right : FacingDirection::Left);
	bubble->AddComponent<PhysicsComponent>();
	bubble->AddComponent<BubbleStateComponent>()->Start();
	bubble->AddComponent<CollisionComponent>(
		glm::vec2{ 16.f, 16.f },
		CollisionType::Bubble
	);

	scene.Add(std::move(bubble));
}
