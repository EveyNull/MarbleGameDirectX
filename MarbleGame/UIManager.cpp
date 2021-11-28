#include "UIManager.h"
#include "Window.h"

UIManager::UIManager(Renderer* renderer)
{
	HRESULT result;

	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
	if (FAILED(result)) return;

	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dwriteFactory), reinterpret_cast<IUnknown**>(&dwriteFactory));
	if (FAILED(result))return;

	result = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
	if(FAILED(result)) return;

	HWND hwnd = Window::MainWindow();
	GetClientRect(hwnd, &clientRect);

	D2D1_SIZE_U size = D2D1::SizeU(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	float dpiX = GetDpiForWindow(hwnd);
	float dpiY = dpiX;

	D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpiX,
		dpiY
	);

	result = d2dFactory->CreateDxgiSurfaceRenderTarget(renderer->GetBackBufferDXGISurface(), &properties, &renderTarget);
	if (FAILED(result)) return;

	result = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &brush);

	text = new Text(dwriteFactory, L"TEST", L"Sans Serif", 72.0f);

	image = new Image(L"earth.jpg", wicFactory, renderTarget);

	D2D1_SIZE_F RTsize = renderTarget->GetSize();
	float x = 100.0f;
	float y = 100.0f;
	float radius = 50.0f;
	for (int i = 0; i < 3; ++i)
	{
		circles[i] = D2D1::Ellipse(D2D1::Point2F(x + (125 * i), y), radius, radius);
	}
}

void UIManager::AddText()
{

}

void UIManager::RenderUI(bool clear)
{
	renderTarget->BeginDraw();
	if (clear)
	{
		renderTarget->Clear(D2D1_COLOR_F({ 182.0f/255.0f, 227.0f/255.0f, 253.0f/255.0f, 1 }));
	}
	renderTarget->SetTransform(D2D1::IdentityMatrix());
	//renderTarget->Clear(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));

	D2D1_RECT_F layout = D2D1::RectF(
		(float)clientRect.right - 300.0f,
		(float)clientRect.top,
		(float)(clientRect.right),
		(float)(clientRect.bottom)
	);

	renderTarget->DrawTextW(
		text->GetText(),
		text->GetLength(),
		text->GetFormat(),
		layout,
		brush

	);
	renderTarget->CreateBitmapBrush(image->GetBitmap(), &bitmapBrush);

	for (int i = 0; i < numLives; ++i)
	{
		float x,y;
		x = circles[i].point.x - circles[i].radiusX;
		y = circles[i].point.y - circles[i].radiusY;
		bitmapBrush->SetTransform(D2D1::Matrix3x2F::Translation(D2D1::SizeF(x, y)));
		renderTarget->FillEllipse(circles[i], bitmapBrush);
	}
	//renderTarget->DrawBitmap(image->GetBitmap());

	renderTarget->EndDraw();

}

void UIManager::UpdateText(const wchar_t* newText)
{
	text->SetText(newText);
}

int UIManager::UpdateLives()
{
	if (numLives <= 0)
	{
		numLives = 3;
	}
	else
	{
		numLives--;
	}
	return numLives;
}