#include <algorithm>
#include "Scene.h"

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	if (m_IsUpdating)
	{
		m_pendingObjects.emplace_back(std::move(object));
		return;
	}

	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(const GameObject& object)
{
	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[&object](const auto& ptr) { return ptr.get() == &object; }
		),
		m_objects.end()
	);
}

void Scene::RemoveAll()
{
	m_objects.clear();
	m_pendingObjects.clear();
}

void Scene::FixedUpdate(float fixedTimeStap)
{
	for (const auto& object : m_objects)
	{
		object->FixedUpdate(fixedTimeStap);
	}
}

void Scene::Update(float deltaTime)
{
	m_IsUpdating = true;
	for(auto& object : m_objects)
	{
		if (!object->IsDestroyed())
			object->Update(deltaTime);
	}
	m_IsUpdating = false;

	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[](const auto& object) { return object->IsDestroyed(); }
		),
		m_objects.end()
	);

	for (auto& object : m_pendingObjects)
	{
		m_objects.emplace_back(std::move(object));
	}
	m_pendingObjects.clear();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		if (!object->IsDestroyed())
			object->Render();
	}
}

