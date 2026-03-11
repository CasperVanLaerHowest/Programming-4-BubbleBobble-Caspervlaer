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
#include "TextComponent.h"
#include "TransformComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "ImGuiComponent.h"
#include "InputManager.h"
#include "MoveCommand.h"

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

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(292, 20, 0);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("FPS: ", font);
	go->AddComponent<dae::FPSComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 20, 0);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(400, 300, 0);
	go->AddComponent<dae::TextureComponent>()->SetTexture("Sprite1.png");
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, std::make_unique<MoveCommand>(go.get(), Direction::LEFT));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, std::make_unique<MoveCommand>(go.get(), Direction::RIGHT));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, std::make_unique<MoveCommand>(go.get(), Direction::UP));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, std::make_unique<MoveCommand>(go.get(), Direction::DOWN));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(450, 300, 0);
	go->AddComponent<dae::TextureComponent>()->SetTexture("Sprite2.png");
	auto cmdLeft = std::make_unique<MoveCommand>(go.get(), Direction::LEFT);
	cmdLeft->SetSpeed(200.f);
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_LEFT, std::move(cmdLeft));

	auto cmdRight = std::make_unique<MoveCommand>(go.get(), Direction::RIGHT);
	cmdRight->SetSpeed(200.f);
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_RIGHT, std::move(cmdRight));

	auto cmdUp = std::make_unique<MoveCommand>(go.get(), Direction::UP);
	cmdUp->SetSpeed(200.f);
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_UP, std::move(cmdUp));

	auto cmdDown = std::make_unique<MoveCommand>(go.get(), Direction::DOWN);
	cmdDown->SetSpeed(200.f);
	dae::InputManager::GetInstance().BindCommand(Inputs::DPAD_DOWN, std::move(cmdDown));
	scene.Add(std::move(go));
	
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
