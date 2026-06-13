#include "BubbleBobbleGame.h"

#include "../Commands/HighScoreInputCommand.h"
#include "../Commands/StartMenuInputCommand.h"
#include "../Components/HighScoreEntryComponent.h"
#include "../Components/StartMenuComponent.h"
#include "../Components/TimedCallbackComponent.h"
#include "../Events/GameEventQueue.h"
#include "../HelperFunctions/GameObjectFactory.h"

#include "GameObject.h"
#include "InputManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Service/AudioLocator.h"

#include <SDL3/SDL.h>

#include <memory>
#include <string>
#include <utility>

BubbleBobbleGame::BubbleBobbleGame(std::filesystem::path dataPath)
	: m_LevelLoader{ std::make_unique<LevelLoader>(std::move(dataPath)) }
{
	GameEventQueue::GetInstance().ClearSubscribers();
	GameEventQueue::GetInstance().Subscribe([this](const GameEvent& event)
		{
			HandleGameEvent(event);
		});
}

BubbleBobbleGame::~BubbleBobbleGame()
{
	GameEventQueue::GetInstance().ClearSubscribers();
}

void BubbleBobbleGame::Load()
{
	LoadStartScreen();
	dae::SceneManager::GetInstance().SetActiveScene("Start");
}

dae::Scene& BubbleBobbleGame::LoadHighScoreEntryScreen(int score)
{
	m_LevelLoader->UnbindGameplayControls();
	auto& scene{ dae::SceneManager::GetInstance().CreateOrClearScene("HighScore") };

	AddText(scene, "GAME OVER", { 380.f, 70.f }, 36);
	AddText(scene, "ENTER YOUR INITIALS", { 380.f, 135.f }, 24);

	dae::TextComponent* nameTextComponent{};
	auto nameText = CreateTextObject("NAME: [A] A  A ", { 380.f, 190.f }, 24, &nameTextComponent);
	dae::TextComponent* statusTextComponent{};
	auto statusText = CreateTextObject("", { 380.f, 240.f }, 14, &statusTextComponent);

	dae::TextComponent* scoresTextComponent{};
	auto scoresText = CreateTextObject("TOP 10", { 380.f, 285.f }, 14, &scoresTextComponent);

	dae::TextComponent* currentScoreTextComponent{};
	auto currentScoreText = CreateTextObject("CURRENT SCORE: " + std::to_string(score), { 380.f, 520.f }, 14, &currentScoreTextComponent);

	auto highScoreEntry = std::make_unique<dae::GameObject>();
	auto* highScoreEntryComponent = highScoreEntry->AddComponent<HighScoreEntryComponent>(m_LevelLoader->GetDataPath(), score);
	highScoreEntryComponent->SetTextComponents(nameTextComponent, statusTextComponent, scoresTextComponent, currentScoreTextComponent);
	highScoreEntryComponent->SetReturnToStartCallback([this]()
		{
			ReturnToStartScreen();
		});

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_UP, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::PreviousCharacter));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::PreviousCharacter));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_DOWN, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::NextCharacter));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::NextCharacter));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_LEFT, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::PreviousSlot));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::PreviousSlot));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_RIGHT, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::NextSlot));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::NextSlot));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_RETURN, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::Confirm));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_SPACE, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::Confirm));
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_UP, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::PreviousCharacter));
	dae::InputManager::GetInstance().BindCommand(Inputs::LEFT_THUMB_UP, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::PreviousCharacter));
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_DOWN, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::NextCharacter));
	dae::InputManager::GetInstance().BindCommand(Inputs::LEFT_THUMB_DOWN, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::NextCharacter));
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_LEFT, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::PreviousSlot));
	dae::InputManager::GetInstance().BindCommand(Inputs::LEFT_THUMB_LEFT, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::PreviousSlot));
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_RIGHT, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::NextSlot));
	dae::InputManager::GetInstance().BindCommand(Inputs::LEFT_THUMB_RIGHT, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::NextSlot));
	dae::InputManager::GetInstance().BindCommand(Inputs::A, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::Confirm));
	dae::InputManager::GetInstance().BindCommand(Inputs::START, std::make_unique<HighScoreInputCommand>(highScoreEntryComponent, HighScoreInputAction::Confirm));

	scene.Add(std::move(nameText));
	scene.Add(std::move(statusText));
	scene.Add(std::move(scoresText));
	scene.Add(std::move(currentScoreText));
	scene.Add(std::move(highScoreEntry));

	return scene;
}

dae::Scene& BubbleBobbleGame::LoadVersusResultScreen(bool playerOneWon)
{
	m_LevelLoader->UnbindGameplayControls();
	auto& scene{ dae::SceneManager::GetInstance().CreateOrClearScene("VersusResult") };

	AddText(scene, playerOneWon ? "PLAYER 1 WINS" : "PLAYER 2 WINS", { 380.f, 220.f }, 36);
	AddText(scene, "RETURNING TO MENU", { 380.f, 290.f }, 18);

	auto timer = std::make_unique<dae::GameObject>();
	timer->AddComponent<TimedCallbackComponent>(5.f, [this]()
		{
			ReturnToStartScreen();
		});
	scene.Add(std::move(timer));

	return scene;
}

void BubbleBobbleGame::ReturnToStartScreen()
{
	m_LevelLoader->UnbindGameplayControls();
	dae::SceneManager::GetInstance().CreateOrClearScene("HighScore");
	dae::SceneManager::GetInstance().CreateOrClearScene("VersusResult");
	m_GameStarted = false;
	m_PlayerScores.fill(0);
	LoadStartScreen();
	dae::SceneManager::GetInstance().SetActiveScene("Start");
}

void BubbleBobbleGame::HandleGameEvent(const GameEvent& event)
{
	if (event.type == GameEventType::VersusFinished)
	{
		AudioLocator::GetAudio().StopMusic();
		m_LevelLoader->StoreCurrentPlayerScores(m_PlayerScores);
		dae::SceneManager::GetInstance().CreateOrClearScene("Level");
		LoadVersusResultScreen(event.playerOneWon);
		dae::SceneManager::GetInstance().SetActiveScene("VersusResult");
		return;
	}

	if (event.type == GameEventType::GameOver)
	{
		AudioLocator::GetAudio().StopMusic();
		AudioLocator::GetAudio().PlaySound("Data/Sound/lose.wav");
		m_LevelLoader->StoreCurrentPlayerScores(m_PlayerScores);
		const int finalScore = m_PlayerScores[0] + m_PlayerScores[1];
		dae::SceneManager::GetInstance().CreateOrClearScene("Level");
		LoadHighScoreEntryScreen(finalScore);
		dae::SceneManager::GetInstance().SetActiveScene("HighScore");
		return;
	}

	m_LevelLoader->StoreCurrentPlayerScores(m_PlayerScores);

	const int levelCount = m_LevelLoader->GetLevelCount();
	if (levelCount <= 0)
		return;

	const int nextLevelIndex = (event.levelIndex + 1) % levelCount;
	AudioLocator::GetAudio().PlaySound("Data/Sound/nextLevel.wav");
	m_LevelLoader->Load({ event.gameMode, nextLevelIndex, m_PlayerScores });
	dae::SceneManager::GetInstance().SetActiveScene("Level");
}

dae::Scene& BubbleBobbleGame::LoadStartScreen()
{
	AudioLocator::GetAudio().PlayMusic("Data/Sound/MainTheme.wav");

	auto& scene{ dae::SceneManager::GetInstance().CreateOrClearScene("Start") };

	AddText(scene, "BUBBLE BOBBLE", { 380.f, 120.f }, 36);

	dae::TextComponent* singlePlayerTextComponent{};
	auto singlePlayerText = CreateTextObject("SINGLE PLAYER", { 380.f, 240.f }, 24, &singlePlayerTextComponent);

	dae::TextComponent* coOpTextComponent{};
	auto coOpText = CreateTextObject("CO-OP", { 380.f, 280.f }, 24, &coOpTextComponent);

	dae::TextComponent* versusTextComponent{};
	auto versusText = CreateTextObject("VERSUS", { 380.f, 320.f }, 24, &versusTextComponent);

	AddText(scene, "SELECT AND PRESS SPACE/A", { 380.f, 380.f }, 18);

	auto startMenu = std::make_unique<dae::GameObject>();
	auto* startMenuComponent = startMenu->AddComponent<StartMenuComponent>([this](GameMode gameMode)
		{
			StartGame(gameMode);
		});
	startMenuComponent->SetTextComponents(singlePlayerTextComponent, coOpTextComponent, versusTextComponent);

	scene.Add(std::move(singlePlayerText));
	scene.Add(std::move(coOpText));
	scene.Add(std::move(versusText));
	scene.Add(std::move(startMenu));

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_UP, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::PreviousOption));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::PreviousOption));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_DOWN, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::NextOption));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::NextOption));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_RETURN, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::Confirm));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_SPACE, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::Confirm));

	for (int controllerIndex = 0; controllerIndex <= 2; ++controllerIndex)
	{
		dae::InputManager::GetInstance().BindCommand(controllerIndex, Inputs::DPAD_UP, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::PreviousOption));
		dae::InputManager::GetInstance().BindCommand(controllerIndex, Inputs::LEFT_THUMB_UP, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::PreviousOption));
		dae::InputManager::GetInstance().BindCommand(controllerIndex, Inputs::DPAD_DOWN, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::NextOption));
		dae::InputManager::GetInstance().BindCommand(controllerIndex, Inputs::LEFT_THUMB_DOWN, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::NextOption));
		dae::InputManager::GetInstance().BindCommand(controllerIndex, Inputs::A, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::Confirm));
		dae::InputManager::GetInstance().BindCommand(controllerIndex, Inputs::START, std::make_unique<StartMenuInputCommand>(startMenuComponent, StartMenuInputAction::Confirm));
	}

	return scene;
}

void BubbleBobbleGame::StartGame(GameMode gameMode)
{
	if (m_GameStarted)
		return;

	m_PlayerScores.fill(0);
	m_GameStarted = true;

	AudioLocator::GetAudio().StopMusic();
	m_LevelLoader->Load({ gameMode, 0, m_PlayerScores });
	dae::SceneManager::GetInstance().SetActiveScene("Level");
}
