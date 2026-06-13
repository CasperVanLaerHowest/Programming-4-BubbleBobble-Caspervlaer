#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate(float fixedTimeStap)
{
	if (m_pActiveScene != nullptr)
		m_pActiveScene->FixedUpdate(fixedTimeStap);
}

void dae::SceneManager::Update(float deltaTime)
{
	if (m_pActiveScene != nullptr)
		m_pActiveScene->Update(deltaTime);
}

void dae::SceneManager::Render()
{
	if (m_pActiveScene != nullptr)
		m_pActiveScene->Render();
}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	auto& scene = *m_scenes.back();
	if (m_pActiveScene == nullptr)
		m_pActiveScene = &scene;

	return scene;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	auto& scene = CreateScene();
	m_namedScenes[name] = &scene;
	return scene;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	if (const auto scene = m_namedScenes.find(name); scene != m_namedScenes.end())
		m_pActiveScene = scene->second;
}
