#include "GameObjectFactory.h"
#include "FruitSpawnRegistry.h"
#include "../Components/CollisionComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/EnemyStateComponent.h"
#include "../Components/EnemyDropComponent.h"
#include "../Components/PlayerStateComponent.h"
#include "../Components/PlayerSlotComponent.h"
#include "../Components/FacingComponent.h"
#include "../Components/MaitaBallComponent.h"
#include "../Components/MaitaPlayerComponent.h"
#include "../Components/MaitaStateComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Commands/MoveCommand.h"
#include "../Commands/JumpCommand.h"
#include "../Commands/ShootMaitaBallCommand.h"

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
#include "../Components/PoppedEnemyComponent.h"
#include "../Observers/AudioObserver.h"
#include <TextureComponent.h>
#include <vector>

static void BindShootCommand(dae::GameObject* player, dae::Scene& scene, SDL_Scancode key, bool shootsMaitaBall)
{
	if (shootsMaitaBall)
	{
		dae::InputManager::GetInstance().BindCommand(key, std::make_unique<ShootMaitaBallCommand>(player, scene));
		return;
	}

	dae::InputManager::GetInstance().BindCommand(key, std::make_unique<ShootBubbleCommand>(player, scene));
}

static void BindShootCommand(dae::GameObject* player, dae::Scene& scene, int controllerIndex, Inputs input, bool shootsMaitaBall)
{
	if (shootsMaitaBall)
	{
		dae::InputManager::GetInstance().BindCommand(controllerIndex, input, std::make_unique<ShootMaitaBallCommand>(player, scene));
		return;
	}

	dae::InputManager::GetInstance().BindCommand(controllerIndex, input, std::make_unique<ShootBubbleCommand>(player, scene));
}

static std::vector<std::shared_ptr<dae::Texture2D>> LoadTextures(const std::vector<std::string>& files)
{
	std::vector<std::shared_ptr<dae::Texture2D>> textures{};
	textures.reserve(files.size());

	for (const auto& file : files)
	{
		textures.push_back(dae::ResourceManager::GetInstance().LoadTexture(file));
	}

	return textures;
}


void CreatePlayer(dae::Scene& scene, const PlayerSettings& settings) {
	auto player{ std::make_unique<dae::GameObject>() };
	player->GetComponent<dae::TransformComponent>()->SetLocalPosition(settings.spawnPosition.x, settings.spawnPosition.y, 0);
	player->GetComponent<dae::TransformComponent>()->SetScale(settings.scale.x, settings.scale.y, 1);
	player->AddComponent<PhysicsComponent>();
	player->AddComponent<CollisionComponent>(glm::vec2{ 20, 20 }, CollisionType::Player);
	player->AddComponent<FacingComponent>();
	player->AddComponent<PlayerSlotComponent>(settings.playerIndex);
	if (settings.isMaitaPlayer)
	{
		player->AddComponent<MaitaPlayerComponent>();
	}

	const auto idleFrames = LoadTextures(settings.idleFrames);
	const auto dieFrames = LoadTextures(settings.dieFrames);
	const auto shootFrames = LoadTextures(settings.shootFrames);
	const auto walkFrames = LoadTextures(settings.walkFrames);

	auto animationComp = player->AddComponent<AnimationComponent>();
	animationComp->AddAnimation("Idle", idleFrames, 0.5f, true);
	animationComp->AddAnimation("Die", dieFrames, 0.3f, false);
	animationComp->AddAnimation("Shoot", shootFrames, 0.2f, false);
	animationComp->AddAnimation("Walk", walkFrames, 0.2f, true);

	player->AddComponent<PlayerStateComponent>(settings.spawnPosition);

	auto healthDisplay{ std::make_unique<dae::GameObject>() };
	healthDisplay->GetComponent<dae::TransformComponent>()->SetLocalPosition(settings.healthPosition.x, settings.healthPosition.y, 0);
	healthDisplay->GetComponent<dae::TransformComponent>()->SetScale(2, 2, 1);
	auto healthObserver = healthDisplay->AddComponent<HealthObserver>("Live.png", settings.health);
	auto audioObserver = player->AddComponent<AudioObserver>();

	auto* healthComponent = player->AddComponent<HealthComponent>(settings.health);
	healthComponent->AddObserver(healthObserver);
	healthComponent->AddObserver(audioObserver);

	auto scoreText{ std::make_unique<dae::GameObject>() };
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	scoreText->GetComponent<dae::TransformComponent>()->SetLocalPosition(settings.scorePosition.x, settings.scorePosition.y, 0);
	scoreText->AddComponent<dae::TextComponent>(std::to_string(settings.initialScore), font);
	auto scoreObserver = scoreText->AddComponent<ScoreObserver>();

	auto playerText{ std::make_unique<dae::GameObject>() };
	playerText->GetComponent<dae::TransformComponent>()->SetLocalPosition(settings.labelPosition.x, settings.labelPosition.y, 0);
	playerText->AddComponent<dae::TextComponent>(settings.label, font);

	player->AddComponent<ScoreComponent>(settings.initialScore)->AddObserver(scoreObserver);

	auto cmdjump = std::make_unique<JumpCommand>(player.get());
	cmdjump->SetJumpStrength(400.f);

	dae::InputManager::GetInstance().BindCommand(settings.controls.left, std::make_unique<MoveCommand>(player.get(), glm::vec2{ -1, 0 }));
	dae::InputManager::GetInstance().BindCommand(settings.controls.right, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 1, 0 }));
	dae::InputManager::GetInstance().BindCommand(settings.controls.jump, std::move(cmdjump));
	dae::InputManager::GetInstance().BindCommand(settings.controls.down, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 0, 1 }));

	BindShootCommand(player.get(), scene, settings.controls.shoot, settings.shootsMaitaBall);

	if (settings.controllerIndex >= 0)
	{
		auto controllerJump = std::make_unique<JumpCommand>(player.get());
		controllerJump->SetJumpStrength(400.f);
		auto controllerDpadJump = std::make_unique<JumpCommand>(player.get());
		controllerDpadJump->SetJumpStrength(400.f);
		auto controllerStickJump = std::make_unique<JumpCommand>(player.get());
		controllerStickJump->SetJumpStrength(400.f);

		dae::InputManager::GetInstance().BindCommand(settings.controllerIndex, Inputs::DPAD_LEFT, std::make_unique<MoveCommand>(player.get(), glm::vec2{ -1, 0 }));
		dae::InputManager::GetInstance().BindCommand(settings.controllerIndex, Inputs::LEFT_THUMB_LEFT, std::make_unique<MoveCommand>(player.get(), glm::vec2{ -1, 0 }));
		dae::InputManager::GetInstance().BindCommand(settings.controllerIndex, Inputs::DPAD_RIGHT, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 1, 0 }));
		dae::InputManager::GetInstance().BindCommand(settings.controllerIndex, Inputs::LEFT_THUMB_RIGHT, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 1, 0 }));
		dae::InputManager::GetInstance().BindCommand(settings.controllerIndex, Inputs::DPAD_DOWN, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 0, 1 }));
		dae::InputManager::GetInstance().BindCommand(settings.controllerIndex, Inputs::LEFT_THUMB_DOWN, std::make_unique<MoveCommand>(player.get(), glm::vec2{ 0, 1 }));
		dae::InputManager::GetInstance().BindCommand(settings.controllerIndex, Inputs::DPAD_UP, std::move(controllerDpadJump));
		dae::InputManager::GetInstance().BindCommand(settings.controllerIndex, Inputs::LEFT_THUMB_UP, std::move(controllerStickJump));
		dae::InputManager::GetInstance().BindCommand(settings.controllerIndex, Inputs::A, std::move(controllerJump));
		BindShootCommand(player.get(), scene, settings.controllerIndex, Inputs::X, settings.shootsMaitaBall);
	}
	scene.Add(std::move(player));
	scene.Add(std::move(healthDisplay));
	scene.Add(std::move(scoreText));
	scene.Add(std::move(playerText));
}

void CreateEnemy(dae::Scene& scene, const glm::vec2& spawnPos) {
	auto enemy = std::make_unique<dae::GameObject>();
	enemy->GetComponent<dae::TransformComponent>()->SetLocalPosition(spawnPos.x, spawnPos.y, 0.f);
	enemy->GetComponent<dae::TransformComponent>()->SetScale(2.f, 2.f, 1.f);
	auto* physics = enemy->AddComponent<PhysicsComponent>();
	physics->SetMaxHorizontalSpeed(120.f);
	enemy->AddComponent<FacingComponent>();
	enemy->AddComponent<CollisionComponent>(
		glm::vec2{ 20.f, 20.f },
		CollisionType::Enemy
	);
	
	std::vector<std::shared_ptr<dae::Texture2D>> walkFrames = {
	dae::ResourceManager::GetInstance().LoadTexture("EnemyMove0.png"),
	dae::ResourceManager::GetInstance().LoadTexture("EnemyMove1.png"),
	dae::ResourceManager::GetInstance().LoadTexture("EnemyMove2.png"),
	dae::ResourceManager::GetInstance().LoadTexture("EnemyMove3.png")
	};

	std::vector<std::shared_ptr<dae::Texture2D>> angryFrames = {
	dae::ResourceManager::GetInstance().LoadTexture("EnemyAngry0.png"),
	dae::ResourceManager::GetInstance().LoadTexture("EnemyAngry1.png"),
	dae::ResourceManager::GetInstance().LoadTexture("EnemyAngry2.png"),
	dae::ResourceManager::GetInstance().LoadTexture("EnemyAngry3.png")
	};

	auto animationComp = enemy->AddComponent<AnimationComponent>();
	animationComp->AddAnimation("Walk", walkFrames, 0.5f, true);
	animationComp->AddAnimation("Angry", angryFrames, 0.25f, true);
	animationComp->PlayAnimation("Walk");
	enemy->AddComponent<EnemyDropComponent>("Fruit0.png", 100);
	enemy->AddComponent<EnemyStateComponent>();

	scene.Add(std::move(enemy));
}

void CreateMaitaEnemy(dae::Scene& scene, const glm::vec2& spawnPos)
{
	auto enemy = std::make_unique<dae::GameObject>();
	enemy->GetComponent<dae::TransformComponent>()->SetLocalPosition(spawnPos.x, spawnPos.y, 0.f);
	enemy->GetComponent<dae::TransformComponent>()->SetScale(2.f, 2.f, 1.f);
	auto* physics = enemy->AddComponent<PhysicsComponent>();
	physics->SetMaxHorizontalSpeed(90.f);
	enemy->AddComponent<FacingComponent>();
	enemy->AddComponent<CollisionComponent>(
		glm::vec2{ 20.f, 20.f },
		CollisionType::Enemy
	);

	auto animationComp = enemy->AddComponent<AnimationComponent>();
	animationComp->AddAnimation("Move", LoadTextures({ "Enemy2Move0.png", "Enemy2Move1.png", "Enemy2Move2.png", "Enemy2Move3.png", "Enemy2Move4.png" }), 0.16f, true);
	animationComp->AddAnimation("Attack", LoadTextures({ "Enemy2Attack0.png", "Enemy2Attack1.png", "Enemy2Attack2.png", "Enemy2Attack3.png", "Enemy2Attack4.png" }), 0.1f, true);
	animationComp->AddAnimation("Angry", LoadTextures({ "Enemy2Angry0.png", "Enemy2Angry1.png", "Enemy2Angry2.png", "Enemy2Angry3.png", "Enemy2Angry4.png" }), 0.12f, true);
	animationComp->AddAnimation("AngryAttack", LoadTextures({ "Enemy2AngryAttack0.png", "Enemy2AngryAttack1.png", "Enemy2AngryAttack2.png", "Enemy2AngryAttack3.png", "Enemy2AngryAttack4.png" }), 0.08f, true);
	animationComp->PlayAnimation("Move");

	enemy->AddComponent<EnemyDropComponent>("Fruit1.png", 200);
	enemy->AddComponent<MaitaStateComponent>(scene);

	scene.Add(std::move(enemy));
}

void AddText(dae::Scene& scene, const std::string& text, const glm::vec2& position, uint8_t fontSize)
{
	scene.Add(CreateTextObject(text, position, fontSize));
}

std::unique_ptr<dae::GameObject> CreateTextObject(const std::string& text, const glm::vec2& position, uint8_t fontSize, dae::TextComponent** textComponent)
{
	auto go = std::make_unique<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0.f);
	auto* component = go->AddComponent<dae::TextComponent>(text, font);

	if (textComponent != nullptr)
	{
		*textComponent = component;
	}

	return go;
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

	std::vector<std::shared_ptr<dae::Texture2D>> bubbleRedFrames = {
	dae::ResourceManager::GetInstance().LoadTexture("BubbleRed0.png"),
	dae::ResourceManager::GetInstance().LoadTexture("BubbleRed1.png"),
	dae::ResourceManager::GetInstance().LoadTexture("BubbleRed2.png")
	};

	std::vector<std::shared_ptr<dae::Texture2D>> trappedRedFrames = {
	dae::ResourceManager::GetInstance().LoadTexture("EnemyBubbleRed0.png"),
	dae::ResourceManager::GetInstance().LoadTexture("EnemyBubbleRed1.png"),
	dae::ResourceManager::GetInstance().LoadTexture("EnemyBubbleRed2.png")
	};

	auto animationComp = bubble->AddComponent<AnimationComponent>();
	animationComp->AddAnimation("Bubble", BubbleFrames, 0.1f, false);
	animationComp->AddAnimation("BubbleRed", bubbleRedFrames, 0.2f, true);
	animationComp->AddAnimation("Trapped", trappedFrames, 0.5f, true);
	animationComp->AddAnimation("TrappedRed", trappedRedFrames, 0.2f, true);
	bubble->GetComponent<AnimationComponent>()->PlayAnimation("Bubble");
	bubble->AddComponent<FacingComponent>()->SetFacingDirection(facingRight ? FacingDirection::Right : FacingDirection::Left);
	bubble->AddComponent<PhysicsComponent>();
	bubble->AddComponent<BubbleStateComponent>(scene)->Start();
	bubble->AddComponent<CollisionComponent>(
		glm::vec2{ 16.f, 16.f },
		CollisionType::Bubble
	);

	scene.Add(std::move(bubble));
}

void SpawnMaitaBall(dae::Scene& scene, const glm::vec2& spawnPos, float direction, int ownerPlayerIndex)
{
	auto ball = std::make_unique<dae::GameObject>();
	ball->GetComponent<dae::TransformComponent>()->SetLocalPosition(spawnPos.x, spawnPos.y, 0.f);
	ball->GetComponent<dae::TransformComponent>()->SetScale(2.f, 2.f, 1.f);
	ball->AddComponent<FacingComponent>()->SetFacingDirection(direction >= 0.f ? FacingDirection::Right : FacingDirection::Left);

	auto animationComp = ball->AddComponent<AnimationComponent>();
	animationComp->AddAnimation("Move", LoadTextures({ "Ball0.png", "Ball1.png", "Ball2.png", "Ball3.png" }), 0.1f, true);
	animationComp->AddAnimation("Break", LoadTextures({ "BallBreak0.png", "BallBreak1.png", "BallBreak2.png" }), 0.08f, false);
	animationComp->PlayAnimation("Move");

	auto* physics = ball->AddComponent<PhysicsComponent>();
	physics->SetMaxHorizontalSpeed(90.f);
	physics->SetFriction(0.f, 0.f);
	physics->SetVelocity({ direction >= 0.f ? 90.f : -90.f, 0.f });
	ball->AddComponent<CollisionComponent>(glm::vec2{ 16.f, 16.f }, CollisionType::MaitaBall);
	ball->AddComponent<MaitaBallComponent>(ownerPlayerIndex);

	scene.Add(std::move(ball));
}

void SpawnFruit(dae::Scene& scene, const glm::vec2& spawnPos)
{
	SpawnFruit(scene, spawnPos, "Fruit0.png", 100);
}

void SpawnFruit(dae::Scene& scene, const glm::vec2& spawnPos, const std::string& texture, int scoreValue)
{
	auto fruit = std::make_unique<dae::GameObject>();
	fruit->GetComponent<dae::TransformComponent>()->SetLocalPosition(spawnPos.x, spawnPos.y, 0.f);
	fruit->GetComponent<dae::TransformComponent>()->SetScale(2.f, 2.f, 1.f);
	fruit->AddComponent<dae::TextureComponent>()->SetTexture("EnemyMove0.png");
	fruit->AddComponent<PoppedEnemyComponent>(FruitSpawnRegistry::GetInstance().GetClosestSpawnPoint(spawnPos), texture, scoreValue);

	scene.Add(std::move(fruit));
}
