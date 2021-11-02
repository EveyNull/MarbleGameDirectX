#include "MeshComponent.h"

void MeshComponent::MakeCube(ID3D11Device* device, float height, float width, float length)
{
	VECTOR3 FBL = { -height/2, -width/2, length/2 };
	VECTOR3 FTL = { -height / 2, width / 2, length / 2 };
	VECTOR3 FTR = { height / 2, width / 2, length / 2 };
	VECTOR3 FBR = { height / 2, -width / 2, length / 2 };
	VECTOR3 BBL= { -height / 2, -width / 2, -length / 2 };
	VECTOR3 BTL = { -height / 2, width / 2, -length / 2 };
	VECTOR3 BTR = { height / 2, width / 2, -length / 2 };
	VECTOR3 BBR = { height / 2, -width / 2, -length / 2 };

	VECTOR3 cube_data[] =
	{
		FBL, FTL, FTR,
		FTR, FBR, FBL,

		FTL, BTL, BTR,
		BTR, FTR, FTL,

		BBL, BTL, BTR,
		BTR, BBR, BBL,

		BBL, FBL, FBR,
		FBR, BBR, BBL,

		BBL, BTL, FTL,
		FTL, FBL, BBL,

		FBR, FTR, BTR,
		BTR, BBR, FBR
	};

	VertexConfig* vertices = new VertexConfig[36];

	for (int i = 0; i < 36; ++i)
	{
		vertices[i].position = cube_data[i];
		if (i % 6 == 0)vertices[i].textureUV = { 0.0f, 0.0f };
		if (i % 6 == 1)vertices[i].textureUV = { 0.0f, 1.0f };
		if (i % 6 == 2)vertices[i].textureUV = { 1.0f, 1.0f };
		if (i % 6 == 3)vertices[i].textureUV = { 1.0f, 1.0f };
		if (i % 6 == 4)vertices[i].textureUV = { 1.0f, 0.0f };
		if (i % 6 == 5)vertices[i].textureUV = { 0.0f, 0.0f };
		vertices[i].normal = cube_data[i];
	}

	InitBuffers(device, vertices, 36);

	delete[] vertices;
	vertices = nullptr;
}

void MeshComponent::MakeSphere(ID3D11Device* device)
{

}

MeshComponent::MeshComponent(HWND& hWnd)
{
	this->hWnd = hWnd;
	indexBuffer = nullptr;
	vertexBuffer = nullptr;

	indexCount = 0;
	vertexCount = 0;

	rotationMatrix = XMMatrixRotationRollPitchYaw(0, 0, 0);
	translationMatrix = XMMatrixTranslation(0, 0, 0);
}

MeshComponent::~MeshComponent()
{
	if (indexBuffer)
	{
		indexBuffer->Release();
	}
	
	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}

	if (texture)
	{
		delete texture;
	}
}

int MeshComponent::GetIndexCount()
{
	return indexCount;
}

ID3D11ShaderResourceView* MeshComponent::GetTexture()
{
	return texture->GetTexture();
}

void MeshComponent::LoadTexture(ID3D11Device* device, const wchar_t* fileName)
{
	texture = new Texture();
	texture->InitTexture(device, fileName);
}

bool MeshComponent::InitBuffers(ID3D11Device* device, VertexConfig* vertices, int vertexCount)
{
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	indexCount = vertexCount;

	indices = new unsigned long[indexCount];
	if (!indices) return false;

	for (int i = 0; i < vertexCount; ++i)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexConfig) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	if (FAILED(result))
	{
		MessageBox(hWnd, L"Failed to create vertex buffer", L"FAILED", MB_ICONERROR | MB_OK);
		return false;
	}
	
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		MessageBox(hWnd, L"Failed to create index buffer", L"FAILED", MB_ICONERROR | MB_OK);
		return false;
	}

	delete[] indices;
	indices = 0;

	return true;
}

void MeshComponent::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

void MeshComponent::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexConfig);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

XMMATRIX MeshComponent::GetRotationMatrix()
{
	return rotationMatrix;
}

XMMATRIX MeshComponent::GetTranslationMatrix()
{
	return translationMatrix;
}

void MeshComponent::TranslateMesh(VECTOR3 position)
{
	translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);
}

void MeshComponent::RotateMesh(VECTOR3 rotation)
{
	rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
}
