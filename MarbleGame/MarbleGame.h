#pragma once
#include <Windows.h>
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

	Window* mainWindow;
	Scene* scene;

	HINSTANCE hInt;

	bool full_screen = false;
	int window_width, window_height;

	unsigned long long startTime = 0;
	unsigned long long prevTime = 0;

	void InitWindow(HINSTANCE&);
};

static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
static MarbleGame* _Game;