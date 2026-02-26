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

		// World position (calculated from parent + relative)
		const glm::vec3& GetWorldPosition();
		
		// Local/Relative position (offset from parent)
		void SetLocalPosition(float x, float y, float z);
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }

		// Convenience - GetPosition returns world position
		const glm::vec3& GetPosition() { return GetWorldPosition(); }

		void SetScale(float x, float y, float z);
		const glm::vec3& GetScale();

		void SetRotation(float x, float y, float z);
		const glm::vec3& GetRotation();

		bool IsDirty() const { return m_IsDirty; }
		void SetDirtyFlag(bool flag) { m_IsDirty = flag; }

	private:
		void UpdateWorldPosition();

		glm::vec3 m_LocalPosition{ 0.f, 0.f, 0.f };   // Position relative to parent
		glm::vec3 m_WorldPosition{ 0.f, 0.f, 0.f };   // Cached world position
		glm::vec3 m_Scale{ 1.f, 1.f, 1.f };
		glm::vec3 m_Rotation{ 0.f, 0.f, 0.f };

		bool m_IsDirty = true;
	};
}
