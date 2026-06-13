#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "Scene.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();
		Scene& CreateScene(const std::string& name);
		void SetActiveScene(const std::string& name);

		void FixedUpdate(float fixedTimeStap);
		void Update(float deltaTime);
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes{};
		std::unordered_map<std::string, Scene*> m_namedScenes{};
		Scene* m_pActiveScene{};
	};
}
