#pragma once
#include "Component.h"
#include "Subject.h"

class HealthComponent :
	public dae::Component,
	public dae::Subject
{
public:
	static constexpr dae::ComponentTypeID StaticTypeID{ 7 };

	HealthComponent(dae::GameObject* owner, int health);
	virtual ~HealthComponent() = default;
	void FixedUpdate(float) override;
	void Update(float) override {}
	void Render() const override {}

	void TakeDamage();
	int GetHealth() const { return m_Health; }

private:
	int m_Health;
	int m_MaxHealth;

	bool m_DirtyFlag = true;
};