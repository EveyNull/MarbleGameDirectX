#include "InputManager.h"
#include "MathHelper.h"

InputManager* InputManager::instance;

InputManager* InputManager::Instance()
{
	return instance;
}

InputManager::InputManager()
{
	instance = this;
}

void InputManager::AddInputMapping(unsigned int key, InputAxis axis, int state)
{
	keyMap[key] = std::pair<InputAxis, int>{axis, state};
}

void InputManager::SetDefaultMousePos(const VECTOR2& screen, const VECTOR2& window)
{
	screenMouseCenter = screen;
	windowMouseCenter = window;
}

void InputManager::SetAxis(unsigned int key, KeyState state)
{
	if (keyMap.count(key))
	{
		InputAxis axis = keyMap[key].first;
		inputMap[axis] = (state == KeyState::DOWN ? keyMap[key].second : 0);
	}
}

void InputManager::SetMouseMovement(const VECTOR2& movement)
{
	mouseMovement = windowMouseCenter - movement;
}

VECTOR2 InputManager::GetDefaultMousePos()
{
	return screenMouseCenter;
}

int InputManager::GetControlState(InputAxis axis)
{
	if (inputMap.count(axis))
	{
		return inputMap[axis];
	}
	return 0;
}

VECTOR2 InputManager::GetMouseMovement()
{
	return mouseMovement;
}