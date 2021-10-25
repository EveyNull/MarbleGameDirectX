#include "InputManager.h"

InputManager* InputManager::instance;

InputManager* InputManager::Instance()
{
	return instance;
}

InputManager::InputManager()
{
	instance = this;
}

void InputManager::SetKey(unsigned int key, KeyState state)
{
	keyMap[key] = state;
}

KeyState InputManager::GetKeyState(unsigned int key)
{
	auto it = keyMap.find(key);
	if (it != keyMap.end())
	{
		return it->second;
	}
	return KeyState::UP;
}
