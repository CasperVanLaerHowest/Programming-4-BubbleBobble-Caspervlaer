#include <SDL3/SDL.h>
#include "AnimationComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "Texture2D.h"

	AnimationComponent::AnimationComponent(dae::GameObject* owner)
		: Component(owner)
	{
	}

	void AnimationComponent::Update(float deltatime)
	{
		if (!m_IsPlaying || m_CurrentAnimationName.empty())
			return;

		auto& animation = m_Animations[m_CurrentAnimationName];
		if (animation.frames.empty())
			return;

		m_AccumulatedTime += deltatime;

		if (m_AccumulatedTime >= animation.frameDuration)
		{
			m_AccumulatedTime -= animation.frameDuration;
			m_CurrentFrameIndex++;

			if (m_CurrentFrameIndex >= static_cast<int>(animation.frames.size()))
			{
				if (animation.loop)
				{
					m_CurrentFrameIndex = 0;
				}
				else
				{
					m_CurrentFrameIndex = static_cast<int>(animation.frames.size()) - 1;
					m_IsPlaying = false;
				}
			}
		}
	}

	void AnimationComponent::Render() const
	{
		auto currentTexture = GetCurrentTexture();
		if (!currentTexture)
			return;

		auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
		const auto& pos = transform->GetWorldPosition();
		const auto& scale = transform->GetScale();

		const auto size = currentTexture->GetSize();
		const float width = size.x * scale.x;
		const float height = size.y * scale.y;

		const SDL_FlipMode flipMode = m_FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		dae::Renderer::GetInstance().RenderTexture(*currentTexture, pos.x, pos.y, width, height, flipMode);
	}

	void AnimationComponent::AddAnimation(const std::string& name, const std::vector<std::shared_ptr<Texture2D>>& frames, float frameDuration, bool loop)
	{
		m_Animations[name] = { frames, frameDuration, loop };
	}

	void AnimationComponent::PlayAnimation(const std::string& name)
	{
		if (m_CurrentAnimationName == name)
			return; // We're already playing this animation

		if (m_Animations.contains(name))
		{
			m_CurrentAnimationName = name;
			m_CurrentFrameIndex = 0;
			m_AccumulatedTime = 0.f;
			m_IsPlaying = true;
		}
	}

	std::shared_ptr<Texture2D> AnimationComponent::GetCurrentTexture() const
	{
		if (m_CurrentAnimationName.empty() || m_Animations.empty())
			return nullptr;

		const auto& animation = m_Animations.at(m_CurrentAnimationName);
		if (m_CurrentFrameIndex < static_cast<int>(animation.frames.size()))
		{
			return animation.frames[m_CurrentFrameIndex];
		}

		return nullptr;
	}

	void AnimationComponent::SetFlipHorizontal(bool flip)
	{
		m_FlipHorizontal = flip;
	}

	bool AnimationComponent::IsFlippedHorizontal() const
	{
		return m_FlipHorizontal;
	}