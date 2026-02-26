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

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>()->SetTexture("background.png");
	//go->SetTexture("background.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>()->SetTexture("logo.png");
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(358, 180,0);
	//go->SetTexture("logo.png");
	//go->SetPosition(358, 180);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(292, 20, 0);
	//auto to = std::make_unique<dae::TextObject>("Programming 4 Assignment", font);
	//to->SetColor({ 255, 255, 0, 255 });
	//to->SetPosition(292, 20);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("FPS: ", font);
	go->AddComponent<dae::FPSComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(20, 20, 0);
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
