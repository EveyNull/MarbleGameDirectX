#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>


using namespace DirectX;

struct MatrixBufferType
{
	XMMATRIX rotation;
	XMMATRIX translation;
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

class ColorShader
{
public:
	ColorShader() = default;
	~ColorShader();

	bool Initialize(ID3D11Device*, HWND);
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	
	bool InitializeShader(ID3D11Device*, HWND, LPCWSTR, LPCWSTR);

	bool SetShaderParams(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

	void OutputShaderErrorMessage(ID3D10Blob*, HWND, LPCWSTR);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* buffer;
};
