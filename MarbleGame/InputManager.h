#pragma once
#include <map>

enum class KeyState
{
	UP = 0,
	DOWN = 1,
	HELD = 2
};

class InputManager
{

	public:
		static InputManager* Instance();
		InputManager();
		void SetKey(unsigned int, KeyState);
		KeyState GetKeyState(unsigned int);
	private:
		static InputManager* instance;
		std::map<unsigned int, KeyState> keyMap;
		
};
