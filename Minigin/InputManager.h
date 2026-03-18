#pragma once
#include "Singleton.h"
#include <memory>
#include "Command.h"
#include <map>
#include <SDL3/SDL.h>
#include "ControllerInput.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void BindCommand(SDL_Scancode key, std::unique_ptr <Command> command);
		void BindCommand(Inputs input, std::unique_ptr <Command> command);
		void UnBindCommand(SDL_Scancode key);
		void UnBindCommand(Inputs input);
		bool IsPressed(SDL_Scancode key);
		bool IsPressed(Inputs input);
	private:
		std::map<SDL_Scancode, std::unique_ptr<Command>> m_Commands;
		std::map<Inputs, std::unique_ptr<Command>> m_ControllerCommands;
		std::unique_ptr<ControllerInput> m_ControllerInput{ std::make_unique<ControllerInput>() };
		std::map<Inputs, bool> m_ControllerInputState;
		std::map<Inputs, bool> m_PreviousControllerInputState;
		std::map<SDL_Scancode, bool> m_KeyboardInputState;
		std::map<SDL_Scancode, bool> m_PreviousKeyboardInputState;

		void ExecuteCommand();
		void UpdateKeyboardInputState();
		void UpdateControllerInputState();
	};

}
