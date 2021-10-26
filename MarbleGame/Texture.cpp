#include "Texture.h"

Texture::~Texture()
{
	textureView->Release();
}

void Texture::InitTexture(ID3D11Device* device, const wchar_t* fileName)
{
	HRESULT result = CreateWICTextureFromFile(device, fileName, NULL, &textureView);
	if (FAILED(result))
	{
		return;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return textureView;
}