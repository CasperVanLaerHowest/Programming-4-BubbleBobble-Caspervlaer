#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Game/BubbleBobbleGame.h"
#include "Minigin.h"

#include <filesystem>

int main(int, char*[])
{
#if __EMSCRIPTEN__
	std::filesystem::path dataLocation = "";
#else
	std::filesystem::path dataLocation = "./Data/";
	if (!std::filesystem::exists(dataLocation))
		dataLocation = "../Data/";
#endif

	dae::Minigin engine(dataLocation);
	BubbleBobbleGame game(dataLocation);
	engine.Run([&game]()
		{
			game.Load();
		});
	return 0;
}
