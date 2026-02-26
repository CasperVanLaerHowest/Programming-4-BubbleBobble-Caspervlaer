#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

void dae::FPSComponent::Update(float deltatime)
{
	CalculateFPS(deltatime);
}

void dae::FPSComponent::UpdateFPS()
{
	GetOwner()->GetComponent<TextComponent>()->SetText("FPS: " + std::to_string(static_cast<int>(m_currentFPS)));
}

void dae::FPSComponent::CalculateFPS(float deltaTime)
{
	fpsTimer += deltaTime;
	++frameCount;

	if (fpsTimer >= 0.5f)
	{
		m_currentFPS = static_cast<float>(frameCount) / fpsTimer;
		frameCount = 0;
		fpsTimer -= 0.5f;
		UpdateFPS();
	}
}