#pragma once
#include "AudioManager.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "UIManager.h"
#include <vector>
class Scene
{
public:
	Scene(HWND hWnd, ID3D11Device* device, int sceneNumber, AudioManager* audio, UIManager* ui);
	~Scene();
	int Update(float dt);
	void Render(Renderer*);
private:
	GameObject* skyBox;
	GameObject* mainCamera;
	GameObject* directionalLight;
	GameObject* playerSphere;

	UIManager* uiManager;

	GameObject** levelGeometry;
	int geometryNumber;

	AudioManager* audioManager;
	CollisionManager* collisionManager;

	bool falling;
	float respawnTimer = 0.0f;

	int sceneNumber;
};

