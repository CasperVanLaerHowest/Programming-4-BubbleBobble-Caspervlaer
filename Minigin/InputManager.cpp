#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			return false;
		}
		if (e.type == SDL_EVENT_KEY_DOWN) {
			
		}
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			
		}
		// etc...

		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	ExecuteCommand();

	return true;
}

void dae::InputManager::BindCommand(SDL_Scancode key, std::unique_ptr <Command> command)
{
	m_Commands[key] = std::move(command);
	const bool isPressed = IsPressed(key);
	m_KeyboardInputState[key] = isPressed;
	m_PreviousKeyboardInputState[key] = isPressed;
}

void dae::InputManager::BindCommand(Inputs input, std::unique_ptr <Command> command)
{
	BindCommand(0, input, std::move(command));
}

void dae::InputManager::BindCommand(int controllerIndex, Inputs input, std::unique_ptr <Command> command)
{
	const ControllerBinding binding{ controllerIndex, input };
	m_ControllerCommands[binding] = std::move(command);
	const bool isPressed = IsPressed(controllerIndex, input);
	m_ControllerInputState[binding] = isPressed;
	m_PreviousControllerInputState[binding] = isPressed;
}

void dae::InputManager::UnBindCommand(SDL_Scancode key)
{
	m_Commands.erase(key);
	m_KeyboardInputState.erase(key);
	m_PreviousKeyboardInputState.erase(key);
}

void dae::InputManager::UnBindCommand(Inputs input)
{
	UnBindCommand(0, input);
}

void dae::InputManager::UnBindCommand(int controllerIndex, Inputs input)
{
	const ControllerBinding binding{ controllerIndex, input };
	m_ControllerCommands.erase(binding);
	m_ControllerInputState.erase(binding);
	m_PreviousControllerInputState.erase(binding);
}

bool dae::InputManager::IsPressed(SDL_Scancode key)
{
	const bool* state = SDL_GetKeyboardState(NULL);
	if (state[key]) {
		return true;
	}
	return false;
}

bool dae::InputManager::IsPressed(Inputs input)
{
	return IsPressed(0, input);
}

bool dae::InputManager::IsPressed(int controllerIndex, Inputs input)
{
	if (m_ControllerInput->IsPressed(input, controllerIndex)) {
		return true;
	}
	return false;
}

bool dae::InputManager::IsControllerConnected(int controllerIndex) const
{
	return m_ControllerInput->IsConnected(controllerIndex);
}

int dae::InputManager::GetConnectedControllerCount() const
{
	int connectedControllerCount{};
	for (int controllerIndex = 0; controllerIndex < 4; ++controllerIndex)
	{
		if (IsControllerConnected(controllerIndex))
		{
			++connectedControllerCount;
		}
	}
	return connectedControllerCount;
}

void dae::InputManager::ExecuteCommand()
{
	UpdateKeyboardInputState();
	UpdateControllerInputState();

	for (auto& command : m_Commands)
	{
		if (IsPressed(command.first)) {
			command.second->Execute(m_PreviousKeyboardInputState[command.first]);
		}
	}

	for (auto& command : m_ControllerCommands)
	{
		if (IsPressed(command.first.first, command.first.second)) {
			command.second->Execute(m_PreviousControllerInputState[command.first]);
		}
	}
}

void dae::InputManager::UpdateKeyboardInputState()
{
	const bool* state = SDL_GetKeyboardState(NULL);
	for (auto& key : m_KeyboardInputState)
	{
		m_PreviousKeyboardInputState[key.first] = key.second;
		key.second = state[key.first];
	}
}

void dae::InputManager::UpdateControllerInputState()
{
	for (auto& input : m_ControllerInputState)
	{
		m_PreviousControllerInputState[input.first] = input.second;
		input.second = m_ControllerInput->IsPressed(input.first.second, input.first.first);
	}
}
