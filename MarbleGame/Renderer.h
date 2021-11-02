#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include <d3d11.h>
#include <vector>
#include "CameraComponent.h"
#include "MeshComponent.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "Shader.h"

using namespace DirectX;	

class Renderer
{
public:
	static Renderer* Instance();
	Renderer();
	~Renderer();
	void Init(HWND, int&, int&, bool);
	void Render(CameraComponent*, std::vector<MeshComponent*>&, LightComponent*);
	ID3D11Device* GetRenderDevice();
	ID3D11DeviceContext* GetDeviceContext();

private:
	static Renderer* instance;
	Shader* shader;

	IDXGISwapChain* swapChain;
	ID3D11RasterizerState* rasterizer;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* backBuffer;
	XMMATRIX m_orthomatrix;
	XMMATRIX m_projectionMatrix;
	XMMATRIX worldMatrix;
};

#endif