#include "ControllerInput.h"

#ifdef _WIN32
#include <Xinput.h>

class ControllerInput::Impl
{
private:
	XINPUT_STATE m_State;
public:
	Impl() {
		ZeroMemory(&m_State, sizeof(XINPUT_STATE));
	};
	~Impl() = default;

	bool IsPressed(Inputs input) {
		
		ZeroMemory(&m_State, sizeof(XINPUT_STATE));
		DWORD dwResult = XInputGetState(0, &m_State);
		if (dwResult == ERROR_SUCCESS)
		{
			switch (input)
			{
			case Inputs::A:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
			case Inputs::B:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
			case Inputs::X:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
			case Inputs::Y:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
			case Inputs::DPAD_UP:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
			case Inputs::DPAD_DOWN:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
			case Inputs::DPAD_LEFT:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
			case Inputs::DPAD_RIGHT:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
			case Inputs::START:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
			case Inputs::BACK:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
			case Inputs::LEFT_THUMB_UP:
				return m_State.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			case Inputs::LEFT_THUMB_DOWN:
				return m_State.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			case Inputs::LEFT_THUMB_LEFT:
				return m_State.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			case Inputs::LEFT_THUMB_RIGHT:
				return m_State.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			case Inputs::RIGHT_THUMB_UP:
				return m_State.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
			case Inputs::RIGHT_THUMB_DOWN:
				return m_State.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
			case Inputs::RIGHT_THUMB_LEFT:
				return m_State.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
			case Inputs::RIGHT_THUMB_RIGHT:
				return m_State.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
			case Inputs::LEFT_SHOULDER:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
			case Inputs::RIGHT_SHOULDER:
				return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
			default:
				return false;
			}
		}
		return false;
	}
};

ControllerInput::ControllerInput() : m_pImpl(std::make_unique<Impl>()) {}

#else
// Non-Windows stub: no XInput support
class ControllerInput::Impl {};

ControllerInput::ControllerInput() : m_pImpl(std::make_unique<Impl>()) {}

#endif

ControllerInput::~ControllerInput() = default;

ControllerInput::ControllerInput(ControllerInput&& other) noexcept = default;

ControllerInput& ControllerInput::operator=(ControllerInput&& other) noexcept = default;

bool ControllerInput::IsPressed([[maybe_unused]] Inputs input)
{
#ifdef _WIN32
	if (m_pImpl) {
		return m_pImpl->IsPressed(input);
	}
#endif
	return false;
}