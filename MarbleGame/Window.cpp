#include "Window.h"


HWND Window::main;

HWND Window::MainWindow()
{
	return main;
}

HWND& Window::GetHandler()
{
	return window;
}

VECTOR2 Window::GetWindowSize()
{
	VECTOR2 size = { window_width, window_height };
	return size;
}

void Window::SetMainWindow()
{
	main = window;
}

void Window::Init(HINSTANCE hInstance, WNDPROC proc, bool fullScreen)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = proc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	int pos_x = 0, pos_y = 0;

	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);

	RECT wr = { 0, 0, screen_width, screen_height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	if (fullScreen)
	{
		DEVMODE dm;
		window_width = screen_width;
		window_height = screen_height;

		memset(&dm, 0, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = (unsigned long)screen_width;
		dm.dmPelsHeight = (unsigned long)screen_height;
		dm.dmBitsPerPel = 32;
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dm, CDS_FULLSCREEN);

	}

	else
	{
		window_width = 1024;
		window_height = 768;
		pos_x = (screen_width - window_width) / 2;
		pos_y = (screen_height - window_height) / 2;
	}

	window = CreateWindowEx(NULL,
		L"WindowClass",
		L"Marble Game",
		WS_OVERLAPPEDWINDOW,
		pos_x,
		pos_y,
		window_width,
		window_height,
		NULL,
		NULL,
		hInstance,
		NULL);
}
