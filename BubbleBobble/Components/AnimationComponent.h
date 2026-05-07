#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace dae { class Texture2D; }
using Texture2D = dae::Texture2D;

struct AnimationCnc
{
	std::vector<std::shared_ptr<Texture2D>> frames;
	float frameDuration{ 0.1f }; 
	bool loop{ true };
};

class AnimationComponent final : public dae::Component
{
public:
	AnimationComponent(dae::GameObject* owner);
	virtual ~AnimationComponent() = default;
	AnimationComponent(const AnimationComponent& other) = delete;
	AnimationComponent(AnimationComponent&& other) = delete;
	AnimationComponent& operator=(const AnimationComponent& other) = delete;
	AnimationComponent& operator=(AnimationComponent&& other) = delete;

	void Update(float deltatime) override;
	void Render() const override;

	void AddAnimation(const std::string& name, const std::vector<std::shared_ptr<Texture2D>>& frames, float frameDuration, bool loop = true);

	void PlayAnimation(const std::string& name);

	std::shared_ptr<Texture2D> GetCurrentTexture() const;

	void SetFlipHorizontal(bool flip);
	bool IsFlippedHorizontal() const;

private:
	std::unordered_map<std::string, AnimationCnc> m_Animations;

	std::string m_CurrentAnimationName{};
	int m_CurrentFrameIndex{ 0 };
	float m_AccumulatedTime{ 0.f };
	bool m_IsPlaying{ false };
	bool m_FlipHorizontal{ false };
};