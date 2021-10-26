#pragma once
#include <d3d11.h>
#include <WICTextureLoader.h>

using namespace DirectX;

class Texture
{
public:
	Texture() = default;
	~Texture();

	void InitTexture(ID3D11Device* device, const wchar_t* fileName);

	ID3D11ShaderResourceView* GetTexture();
private:
	ID3D11ShaderResourceView* textureView;
};