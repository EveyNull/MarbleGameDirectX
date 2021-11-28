#include "MarbleGame.h"
#include "StringHelper.h"
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <windowsx.h>
#include <CoreWindow.h>


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

	window_width = windowSize.x;
	window_height = windowSize.y;
	ShowCursor(false);

	LONG lStyle = GetWindowLong(mainWindow->GetHandler(), GWL_STYLE);
	lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	SetWindowLongPtr(mainWindow->GetHandler(), GWL_STYLE, lStyle);

	renderer->Init(mainWindow->GetHandler(), window_width, window_height, full_screen);

	inputManager = new InputManager();

	SetUpMouseMovement();
	AddKeyMappings();

	audioManager = new AudioManager(3);
	audioManager->LoadWav("go.wav", 0);
	audioManager->LoadWav("cheer.wav", 1);
	audioManager->LoadWav("fall.wav", 2);

	uiManager = new UIManager(renderer);
	uiManager->UpdateText(L"PRESS SPACE TO START");


	return true;
}

void MarbleGame::Run()
{
	MSG msg;

	bool running = true;
	std::clock_t start = std::clock();
	prevTime = 0;

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
		if ((int)inputManager->GetControlState(InputAxis::ESCAPE) > 0)
		{
			PostQuitMessage(0);
		}
		VECTOR2 mousePos = inputManager->GetDefaultMousePos();
		SetCursorPos(mousePos.x, mousePos.y);

		unsigned long long currentTime = std::clock() - start;
		float deltaTime = (currentTime - prevTime);
		
		int sceneChange = 0;
		bool changingLevel = false;
		if (scene)
		{
			sceneChange = scene->Update(deltaTime * 0.001f);
			if (sceneChange != currentLevel)
			{
				currentLevel = sceneChange;
				changingLevel = true;
			}

			prevTime = currentTime;
			scene->Render(renderer);
		}
		else
		{
			if (inputManager->GetControlState(InputAxis::JUMP))
			{
				currentLevel = 1;
				changingLevel = true;
				sceneChange = 1;
				uiManager->UpdateLives();
			}
		}
		uiManager->RenderUI(currentLevel == 0);
		renderer->Present();

		if (changingLevel)
		{
			delete scene;
			if (sceneChange > 0)
			{
				scene = new Scene(mainWindow->GetHandler(), renderer->GetRenderDevice(), currentLevel, audioManager, uiManager);
				if (sceneChange > 1)
				{
					audioManager->PlayWav(1, 0.0f, 0);
				}
				else
				{
					audioManager->PlayWav(0, 0.0f, 0);
				}
			}
			else
			{
				scene = nullptr;
				uiManager->UpdateText(L"PRESS SPACE TO START");
			}
			start = std::clock();
			prevTime = 0;
		}
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
			InputManager::Instance()->SetAxis((unsigned int)wParam, KeyState::UP);
			break;
		}
		case WM_KEYDOWN:
		{
			InputManager::Instance()->SetAxis((unsigned int)wParam, KeyState::DOWN);
			break;
		}
		case WM_MOUSEMOVE:
		{
			VECTOR2 mouseMove =
			{
				(float)GET_X_LPARAM(lParam),
				(float)GET_Y_LPARAM(lParam)
			};
			InputManager::Instance()->SetMouseMovement(mouseMove);
			break;
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

void MarbleGame::AddKeyMappings()
{
	inputManager->AddInputMapping(VK_UP, InputAxis::VERTICAL, 1);
	inputManager->AddInputMapping(0x57, InputAxis::VERTICAL, 1);
	inputManager->AddInputMapping(VK_DOWN, InputAxis::VERTICAL, -1);
	inputManager->AddInputMapping(0x53, InputAxis::VERTICAL, -1);
	inputManager->AddInputMapping(VK_RIGHT, InputAxis::HORIZONTAL, 1);
	inputManager->AddInputMapping(0x44, InputAxis::HORIZONTAL, 1);
	inputManager->AddInputMapping(VK_LEFT, InputAxis::HORIZONTAL, -1);
	inputManager->AddInputMapping(0x41, InputAxis::HORIZONTAL, -1);
	inputManager->AddInputMapping(VK_ESCAPE, InputAxis::ESCAPE, 1);
	inputManager->AddInputMapping(VK_SPACE, InputAxis::JUMP, 1);
}

void MarbleGame::SetUpMouseMovement()
{
	RECT windowPos;
	GetWindowRect(mainWindow->GetHandler(), &windowPos);
	VECTOR2 mousePos =
	{
		windowPos.left + window_width * 0.5f,
		 windowPos.top + window_height * 0.5f
	};
	inputManager->SetDefaultMousePos(mousePos, { 0.5f * (windowPos.right - windowPos.left), (float)window_height / 2 });
	SetCursorPos(mousePos.x, mousePos.y);

}