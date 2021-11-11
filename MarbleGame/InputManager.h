#pragma once
#include "Vector.h"
#include <map>

enum class KeyState
{
	UP = 0,
	DOWN = 1,
	HELD = 2
};

enum class InputAxis
{
	ESCAPE = 0,
	VERTICAL = 1,
	HORIZONTAL = 2,
	JUMP = 3,
};

class InputManager
{

	public:
		static InputManager* Instance();
		InputManager();
		void AddInputMapping(unsigned int key, InputAxis axis, int state);
		void SetDefaultMousePos(const VECTOR2&, const VECTOR2& );
		void SetAxis(unsigned int, KeyState);
		void SetMouseMovement(const VECTOR2&);
		VECTOR2 GetDefaultMousePos();
		int GetControlState(InputAxis axis);
		VECTOR2 GetMouseMovement();
	private:
		static InputManager* instance;
		std::map<unsigned int, std::pair<InputAxis, int>> keyMap;
		std::map<InputAxis, int> inputMap;
		VECTOR2 screenMouseCenter;
		VECTOR2 windowMouseCenter;
		VECTOR2 mouseMovement;
};
