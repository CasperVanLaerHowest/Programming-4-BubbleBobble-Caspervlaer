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
	class GameObject final
	{
	public:
		explicit GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void FixedUpdate(float fixedTimeStap);
		virtual void Update(float deltaTime);
		virtual void Render() const;

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

		template<std::derived_from<Component> T>
		void RemoveComponent()
		{
			constexpr ComponentTypeID id{ T::StaticTypeID };

			if(m_components[id] == nullptr) {
				return;
			}
			m_components[id] = nullptr;
		}

		template<std::derived_from<Component> T>
		T* GetComponent() const
		{
			// Iterate through all components and use dynamic_cast to find a match
			for (const auto& component : m_components)
			{
				if (component != nullptr)
				{
					if (T* casted = dynamic_cast<T*>(component.get()))
					{
						return casted;
					}
				}
			}
			return nullptr;
		}

		template<std::derived_from<Component> T>
		bool HasComponent() const
		{
			return GetComponent<T>() != nullptr;
		}	 

		void SetParent(GameObject* parent, bool keepWorldPosition);
		GameObject* GetParent() const { return m_pParent; }
		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);
		const std::vector<GameObject*>& GetChildren() const { return m_pChildren; }

	private:
		std::array<std::unique_ptr<Component>, MAX_COMPONENTS> m_components{};
		std::vector<GameObject*> m_pChildren{};
		GameObject* m_pParent{ nullptr };
	};
}
