#pragma once

#include "Image.h"
#include "Renderer.h"
#include "Text.h"
#include <d2d1.h>
#include <dwrite.h>
#include <WICTextureLoader.h>
#include <wincodec.h>

#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "Dwrite.lib")

class UIManager
{
public:
	UIManager(Renderer* renderer);
	~UIManager() = default;

	void AddText();
	void RenderUI(bool clear);
	void UpdateText(const wchar_t* newText);
	int UpdateLives();

private:
	RECT clientRect;

	IDWriteFactory* dwriteFactory;
	ID2D1Factory* d2dFactory;
	IWICImagingFactory* wicFactory;


	ID2D1RenderTarget* renderTarget;
	ID2D1SolidColorBrush* brush;
	ID2D1BitmapBrush* bitmapBrush;

	Text* text;

	D2D1_ELLIPSE circles[3];
	Image* image;

	int numLives;
};

