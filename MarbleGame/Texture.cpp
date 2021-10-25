#include "Texture.h"

Texture::~Texture()
{
	texture->Release();
	textureView->Release();
}

void Texture::InitTexture(ID3D11Device* device, const wchar_t* fileName)
{
	size_t maxSize = 2048;
	HRESULT result = CreateWICTextureFromFile(device, fileName, &texture, &textureView, maxSize);
	if (FAILED(result))
	{
		return;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return textureView;
}