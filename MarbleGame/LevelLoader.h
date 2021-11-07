#pragma once
#include "GameObject.h"

class LevelLoader
{
public:
	LevelLoader() = default;
	~LevelLoader() = default;

	GameObject** LoadLevel(int, int&);

private:

};

