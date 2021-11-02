#include "Renderer.h"
#include "Window.h"

#define _USE_MATH_DEFINES
#include <math.h>

Renderer* Renderer::instance;

Renderer* Renderer::Instance()
{
	return instance;
}

Renderer::Renderer()
{
	instance = this;
	swapChain = nullptr;
	device = nullptr;
	context = nullptr;
	backBuffer = nullptr;
	shader = nullptr;

	worldMatrix = XMMATRIX();
}

Renderer::~Renderer()
{
	swapChain->SetFullscreenState(FALSE, NULL);

	swapChain->Release();
	device->Release();
	context->Release();
	backBuffer->Release();
}

void Renderer::Init(HWND hWnd, int& width, int& height, bool fullScreen)
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = Window::MainWindow();
	scd.SampleDesc.Count = 1; 
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;
	//scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		NULL,
		&context
	);


	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	
	HRESULT result = device->CreateRasterizerState(&rasterDesc, &rasterizer);
	if (FAILED(result))
	{
		MessageBox(Window::MainWindow(), L"Failed to initialise rasterizer state", L"ERROR", MB_ICONERROR | MB_OK);
		return;
	}

	context->RSSetState(rasterizer);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	if (FAILED(result))
	{
		MessageBox(Window::MainWindow(), L"Failed to initialise depth buffer", L"ERROR", MB_ICONERROR | MB_OK);
		return;
	}
	device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);


	ID3D11Texture2D* pBackBuffer;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(result))
	{
		MessageBox(Window::MainWindow(), L"Failed to initialise back buffer", L"ERROR", MB_ICONERROR | MB_OK);
		return;
	}

	device->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	pBackBuffer->Release();

	context->OMSetRenderTargets(1, &backBuffer, depthStencilView);

	depthStencilView->Release();
	depthStencilBuffer->Release();

	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));

	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewPort);


	// Setup the projection matrix.
	float fieldOfView = (float)XM_PI / 4.0f;
	float screenAspect = width / height;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 1000.0f);

	// Initialize the world matrix to the identity matrix.
	worldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_orthomatrix = XMMatrixOrthographicLH(width, height, 0.1f , 1000.0f);


	shader = new Shader();
	if (!shader->Initialize(device, hWnd))
	{
		MessageBox(hWnd, L"Could not initialise Color Shader", L"ERROR", MB_OK | MB_ICONERROR);
	}
}

void Renderer::Render(CameraComponent* camera, std::vector<MeshComponent*>& mesh, LightComponent* dirLight)
{

	float color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	context->ClearRenderTargetView(backBuffer, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	camera->Render();
	for (int i = 0; i < 2; ++i)
	{
		MeshComponent* currMesh = mesh[i];
		currMesh->Render(context);

		bool result = shader->Render(context, currMesh->GetIndexCount(), currMesh->GetRotationMatrix(), currMesh->GetTranslationMatrix(), worldMatrix,
			*camera->GetViewMatrix(), m_projectionMatrix, currMesh->GetTexture(), dirLight);
		if (!result)
		{
			MessageBox(Window::MainWindow(), L"Failed to render with shader!", L"ERROR", MB_ICONERROR | MB_OK);
		}
		currMesh = nullptr;
	}
	swapChain->Present(0, 0);

}

ID3D11Device* Renderer::GetRenderDevice()
{
	return device;
}

ID3D11DeviceContext* Renderer::GetDeviceContext()
{
	return context;
}
