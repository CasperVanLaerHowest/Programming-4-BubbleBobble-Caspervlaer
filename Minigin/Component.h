#pragma once
#include <cstdint>

namespace dae
{
	using ComponentTypeID = uint32_t;
	class GameObject;

	class Component
	{
	public:
		Component(GameObject* owner) : m_pOwner(owner) {}

		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void FixedUpdate(float /*fixedTimeStap*/) {}
		virtual void Update(float /*deltatime*/) {}
		virtual void Render() const {}

		GameObject* GetOwner() const noexcept { return m_pOwner; }

	private:
		GameObject* m_pOwner;
	};
}