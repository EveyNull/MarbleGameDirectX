#include <Windows.h>
#include "MarbleGame.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MarbleGame* game = new MarbleGame();
	_Game = game;

	if (game && game->Init(hInstance, lpCmdLine))
	{
		game->Run();
	}
	else
	{
		MessageBox(NULL, L"Failed to create game!", L"ERROR", MB_OK | MB_ICONERROR);
	}
	delete game;

	return 0;
}

