#pragma once
#include <Windows.h>
#include "Vector.h"
class Window
{
public:
	static HWND MainWindow();
	Window() = default;

	HWND& GetHandler();
	VECTOR2 GetWindowSize();

	void SetMainWindow();
	void Init(HINSTANCE, WNDPROC, bool);

private:
	static HWND main;

	HWND window;
	int window_width, window_height;
};
