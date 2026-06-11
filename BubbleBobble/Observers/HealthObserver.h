#pragma once
#include "Component.h"
#include "Observer.h"

#include <memory>
#include <string>

namespace dae
{
	class Texture2D;
}

class HealthObserver :
	public dae::Component,
	public dae::Observer
{
public:
	HealthObserver(dae::GameObject* owner, const std::string& textureFile, int startHealth);
	virtual ~HealthObserver() = default;

	virtual void OnNotify(dae::GameObject& entity, dae::Event event) override;
	virtual void Render() const override;

private:
	void SetHealth(int health);

	std::shared_ptr<dae::Texture2D> m_LifeTexture;
	int m_VisibleLives{};
	float m_Spacing{ 4.f };
};
