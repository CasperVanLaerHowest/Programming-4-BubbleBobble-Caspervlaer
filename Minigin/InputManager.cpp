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
	m_KeyboardInputState[key] = false;
}

void dae::InputManager::BindCommand(Inputs input, std::unique_ptr <Command> command)
{
	m_ControllerCommands[input] = std::move(command);
	m_ControllerInputState[input] = false;
}

void dae::InputManager::UnBindCommand(SDL_Scancode key)
{
	m_Commands.erase(key);
	m_KeyboardInputState.erase(key);
}

void dae::InputManager::UnBindCommand(Inputs input)
{
	m_ControllerCommands.erase(input);
	m_ControllerInputState.erase(input);
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
	if (m_ControllerInput->IsPressed(input)) {
		return true;
	}
	return false;
}

void dae::InputManager::ExecuteCommand()
{
	for (auto& command : m_Commands)
	{
		if (IsPressed(command.first)) {
			command.second->Execute(m_KeyboardInputState[command.first]);
		}
	}

	for (auto& command : m_ControllerCommands)
	{
		if (IsPressed(command.first)) {
			command.second->Execute(true);
		}
	}

	UpdateKeyboardInputState();
	UpdateControllerInputState();
}

void dae::InputManager::UpdateKeyboardInputState()
{
	const bool* state = SDL_GetKeyboardState(NULL);
	for (auto& key : m_KeyboardInputState)
	{
		key.second = state[key.first];
	}
}

void dae::InputManager::UpdateControllerInputState()
{
	for (auto& input : m_ControllerInputState)
	{
		input.second = m_ControllerInput->IsPressed(input.first);
	}
}
