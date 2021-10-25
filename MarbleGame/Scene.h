#pragma once
#include "GameObject.h"
#include "Renderer.h"
#include <vector>
class Scene
{
public:
	Scene(HWND hWnd, ID3D11Device* device);
	void Update(float dt);
	void Render(Renderer*);
private:
	std::vector<GameObject*> gameObjects;
};

