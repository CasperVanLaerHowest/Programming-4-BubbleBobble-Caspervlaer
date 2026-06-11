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
#include "../Components/HealthComponent.h"
#include "../Observers/HealthObserver.h"
#include "../Components/ScoreComponent.h"
#include "../Observers/ScoreObserver.h"
#include "../Components/TextComponent.h"



void CreatePlayer(dae::Scene& scene, const PlayerSettings& settings) {
	auto player{ std::make_unique<dae::GameObject>() };
	player->GetComponent<dae::TransformComponent>()->SetLocalPosition(settings.spawnPosition.x, settings.spawnPosition.y, 0);
	player->GetComponent<dae::TransformComponent>()->SetScale(settings.scale.x, settings.scale.y, 1);
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

	auto healthDisplay{ std::make_unique<dae::GameObject>() };
	healthDisplay->GetComponent<dae::TransformComponent>()->SetLocalPosition(settings.healthPosition.x, settings.healthPosition.y, 0);
	healthDisplay->GetComponent<dae::TransformComponent>()->SetScale(2, 2, 1);
	auto healthObserver = healthDisplay->AddComponent<HealthObserver>("Live.png", settings.health);

	player->AddComponent<HealthComponent>(settings.health)->AddObserver(healthObserver);

	auto scoreText{ std::make_unique<dae::GameObject>() };
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	scoreText->GetComponent<dae::TransformComponent>()->SetLocalPosition(settings.scorePosition.x, settings.scorePosition.y, 0);
	scoreText->AddComponent<dae::TextComponent>("0", font);
	auto scoreObserver = scoreText->AddComponent<ScoreObserver>();

	auto playerText{ std::make_unique<dae::GameObject>() };
	playerText->GetComponent<dae::TransformComponent>()->SetLocalPosition(settings.labelPosition.x, settings.labelPosition.y, 0);
	playerText->AddComponent<dae::TextComponent>(settings.label, font);

	player->AddComponent<ScoreComponent>(0)->AddObserver(scoreObserver);

	auto cmdjump = std::make_unique<JumpCommand>(player.get());
	cmdjump->SetJumpStrength(400.f);

	auto cmdshoot = std::make_unique<ShootBubbleCommand>(player.get(), scene);

	dae::InputManager::GetInstance().BindCommand(settings.controls.left, std::make_unique<MoveCommand>(player.get(), glm::vec2{ -1, 0 }));
	dae::InputManager::GetInstance().BindCommand(settings.controls.right, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 1, 0 }));
	dae::InputManager::GetInstance().BindCommand(settings.controls.jump, std::move(cmdjump));
	dae::InputManager::GetInstance().BindCommand(settings.controls.down, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 0, 1 }));
	dae::InputManager::GetInstance().BindCommand(settings.controls.shoot, std::move(cmdshoot));
	scene.Add(std::move(player));
	scene.Add(std::move(healthDisplay));
	scene.Add(std::move(scoreText));
	scene.Add(std::move(playerText));
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

	std::vector<std::shared_ptr<dae::Texture2D>> trappedFrames = {
	dae::ResourceManager::GetInstance().LoadTexture("EnemyBubble0.png"),
	dae::ResourceManager::GetInstance().LoadTexture("EnemyBubble1.png"),
	dae::ResourceManager::GetInstance().LoadTexture("EnemyBubble2.png")
	};

	auto animationComp = bubble->AddComponent<AnimationComponent>();
	animationComp->AddAnimation("Bubble", BubbleFrames, 0.1f, false);
	animationComp->AddAnimation("Trapped", trappedFrames, 0.1f, true);
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
