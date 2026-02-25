#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include <array>
#include "Component.h"

namespace dae
{
	static constexpr size_t MAX_COMPONENTS{ 32 };

	class Texture2D;
	class GameObject 
	{
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
	public:

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void FixedUpdate(float fixedTimeStap);
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		template<std::derived_from<Component> T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			constexpr ComponentTypeID id{ T::StaticTypeID };

			if (m_components[id] != nullptr) {
				return static_cast<T*>(m_components[id].get());
			}

			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* componentPtr = component.get();

			m_components[id] = std::move(component);
			return componentPtr;
		}

	private:
		std::array<std::unique_ptr<Component>, MAX_COMPONENTS> m_components{};
	};
}
