#include "Image.h"

Image::Image(const wchar_t* fileName, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	HRESULT result = wicFactory->CreateDecoderFromFilename(fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(result)) return;

	LoadBitmapFromFile(fileName, wicFactory, renderTarget);
}

void Image::LoadBitmapFromFile(const wchar_t* fileName, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	IWICBitmapFrameDecode* source;
	IWICStream* stream;
	IWICFormatConverter* converter;

	HRESULT result;

	result = wicFactory->CreateFormatConverter(&converter);
	if (FAILED(result)) return;

	wicFactory->CreateBitmapScaler(&scaler);

	result = decoder->GetFrame(0, &source);
	result = scaler->Initialize(source, 100.0f, 100.0f, WICBitmapInterpolationModeFant);

	result = converter->Initialize(scaler, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
	if (FAILED(result)) return;

	result = renderTarget->CreateBitmapFromWicBitmap(converter, NULL, &bitmap);
	if (FAILED(result)) return;

}

ID2D1Bitmap* Image::GetBitmap()
{
	return bitmap;
}