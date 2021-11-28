#pragma once

#include <d2d1.h>
#include <WICTextureLoader.h>
#include <wincodec.h>

class Image
{
public:
	Image(const wchar_t* fileName, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget);
	~Image() = default;

	ID2D1Bitmap* GetBitmap();

private:
	void LoadBitmapFromFile(const wchar_t* fileName, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget);
	IWICBitmapScaler* scaler;
	IWICBitmapDecoder* decoder;

	ID2D1Bitmap* bitmap;
};

