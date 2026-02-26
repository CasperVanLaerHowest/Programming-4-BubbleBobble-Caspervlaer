#pragma once
#include "Component.h"
#include <glm/ext/vector_float3.hpp>

namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		static constexpr ComponentTypeID StaticTypeID{ 0 };

		TransformComponent(GameObject* owner);
		virtual ~TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		void FixedUpdate([[maybe_unused]] float fixedTimeStap) override {}
		void Update([[maybe_unused]] float deltatime) override {}
		void Render() const override {}

		void SetLocalPosition(float x, float y, float z = 0.f) { m_LocalPosition = { x, y, z }; }
		const glm::vec3& GeLocalPosition() const { return m_LocalPosition; }
		void SetWorldPosition(float x, float y, float z = 0.f) { m_WorldPosition = { x, y, z }; }
		const glm::vec3& GetWorldPosition() const { return m_WorldPosition; }
	private:
		glm::vec3 m_LocalPosition{ 0.f, 0.f, 0.f };
		glm::vec3 m_WorldPosition{ 0.f, 0.f, 0.f };
	};
}
