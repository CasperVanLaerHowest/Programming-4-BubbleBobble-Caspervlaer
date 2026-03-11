#pragma once
#include <memory>
#ifdef _WIN32
#include <Windows.h>
#endif

enum class Inputs
{
	A,
	B,
	X,
	Y,
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	START,
	BACK,
	LEFT_THUMB_UP,
	LEFT_THUMB_DOWN,
	LEFT_THUMB_LEFT,
	LEFT_THUMB_RIGHT,
	RIGHT_THUMB_UP,
	RIGHT_THUMB_DOWN,
	RIGHT_THUMB_LEFT,
	RIGHT_THUMB_RIGHT,
	LEFT_SHOULDER,
	RIGHT_SHOULDER
};

class ControllerInput 
{
public:
	ControllerInput();
	~ControllerInput();

	ControllerInput(const ControllerInput&) = delete;
	ControllerInput& operator=(const ControllerInput&) = delete;
	ControllerInput(ControllerInput&&) noexcept;
	ControllerInput& operator=(ControllerInput&&) noexcept;

	bool IsPressed(Inputs input);

private:
	class Impl;
	std::unique_ptr<Impl> m_pImpl;
};