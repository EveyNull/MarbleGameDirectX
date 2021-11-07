#include "MarbleGame.h"
#include "StringHelper.h"
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>


MarbleGame::~MarbleGame()
{
	DestroyWindow(mainWindow->MainWindow());

	delete renderer;
	delete inputManager;

	renderer  = nullptr;
	inputManager = nullptr;
}

bool MarbleGame::Init(HINSTANCE& hInt, LPSTR& lpStr)
{
	this->hInt = hInt;

	std::vector<std::string> splitstr = StringHelper::split(lpStr, ",");

	if (std::find(splitstr.begin(), splitstr.end(), "FULLSCREEN") != splitstr.end())
	{
		full_screen = true;
	}

	InitWindow(hInt);

	renderer = new Renderer();

	VECTOR2 windowSize = mainWindow->GetWindowSize();

	int window_width = windowSize.x;
	int window_height = windowSize.y;

	renderer->Init(mainWindow->GetHandler(), window_width, window_height, full_screen);

	inputManager = new InputManager();

	scene = new Scene(mainWindow->GetHandler(), renderer->GetRenderDevice());

	return true;
}

void MarbleGame::Run()
{
	MSG msg;

	bool running = true;
	std::clock_t start = std::clock();
	prevTime = start;

	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				running = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (inputManager->GetKeyState(VK_ESCAPE) == KeyState::DOWN)
		{
			PostQuitMessage(0);
		}
		unsigned long long currentTime = std::clock() - start;
		float test = (currentTime - prevTime);
		scene->Update(test * 0.001f);

		prevTime = currentTime;
		scene->Render(renderer);
	}
}

void MarbleGame::InitWindow(HINSTANCE& hInstance)
{
	mainWindow = new Window();
	mainWindow->Init(hInstance, WindowProc, full_screen);
	mainWindow->SetMainWindow();

	ShowWindow(mainWindow->GetHandler(), full_screen ? SW_MAXIMIZE : SW_SHOW);
	SetForegroundWindow(mainWindow->GetHandler());
	SetFocus(mainWindow->GetHandler());
}

LRESULT CALLBACK MarbleGame::HandleMessage(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYUP:
		{
			InputManager::Instance()->SetKey((unsigned int)wParam, KeyState::UP);
			break;
		}
		case WM_KEYDOWN:
		{
			InputManager::Instance()->SetKey((unsigned int)wParam, KeyState::DOWN);
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		
		default:
		{
			return _Game->HandleMessage(hWnd, message, wParam, lParam);
		}
	}

}