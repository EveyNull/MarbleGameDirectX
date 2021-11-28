#pragma once
#include <Windows.h>
#include "AudioManager.h"
#include "UIManager.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Scene.h"
#include "Window.h"

class MarbleGame
{

public:
	MarbleGame() = default;
	~MarbleGame();

	bool Init(HINSTANCE&, LPSTR&);
	void Run();

	LRESULT CALLBACK HandleMessage(
		HWND,
		UINT,
		WPARAM,
		LPARAM);

private:
	Renderer* renderer;
	InputManager* inputManager;
	AudioManager* audioManager;
	UIManager* uiManager;

	Window* mainWindow;
	Scene* scene;

	HINSTANCE hInt;

	bool full_screen = false;
	int window_width, window_height;

	unsigned long long startTime = 0;
	unsigned long long prevTime = 0;

	void InitWindow(HINSTANCE&);
	void SetUpMouseMovement();
	void AddKeyMappings();

	int currentLevel = 0;
};

static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
static MarbleGame* _Game;