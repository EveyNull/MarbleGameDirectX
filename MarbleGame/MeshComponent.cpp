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

		// Top face - 0
		FTL, BTL, BTR,
		BTR, FTR, FTL,


		// Bottom Face - 1
		BBL, FBL, FBR,
		FBR, BBR, BBL,

		// Left face - 2
		BBL, BTL, FTL,
		FTL, FBL, BBL,

		//Front face - 3
		FBL, FTL, FTR,
		FTR, FBR, FBL,

		// Right face - 4
		FBR, FTR, BTR,
		BTR, BBR, FBR,

		// Back face - 5
		BBR, BTR, BTL,
		BTL, BBL, BBR,
	};

	vertices = new VertexConfig[36];


	// Buckle up lads we're gonna UV map
	int face = 0;
	for (int i = 0; i < 36; ++i)
	{
		vertices[i].position = cube_data[i];
		vertices[i].normal = cube_data[i];

		if(face == 0)
		{
			if (i % 6 == 0 || i % 6 == 5) vertices[i].textureUV = { 0.25f, 1.0f / 3.0f };
			else if (i % 6 == 1) vertices[i].textureUV = { 0.25f, 0.0f };
			else if (i % 6 == 2 || i % 6 == 3) vertices[i].textureUV = { 0.5f, 0.0f };
			else vertices[i].textureUV = { 0.5f, 1.0f / 3.0f };
		}
		else if(face == 1)
		{
			if (i % 6 == 0 || i % 6 == 5) vertices[i].textureUV = { 0.25f, 1.0f };
			else if (i % 6 == 1) vertices[i].textureUV = { 0.25f, 2.0f/3.0f };
			else if (i % 6 == 2 || i % 6 == 3) vertices[i].textureUV = { 0.5f, 2.0f/3.0f };
			else vertices[i].textureUV = { 0.5f, 1.0f };
		}
		else
		{
			if (i % 6 == 0 || i % 6 == 5) vertices[i].textureUV = { 0.25f * (face - 2), 2.0f / 3.0f };
			else if (i % 6 == 1) vertices[i].textureUV = { 0.25f * (face - 2), 1.0f / 3.0f };
			else if (i % 6 == 2 || i % 6 == 3) vertices[i].textureUV = { 0.25f * ((face - 2) + 1), 1.0f / 3.0f };
			else vertices[i].textureUV = { 0.25f * ((face - 2) + 1), 2.0f / 3.0f };

		}
		if (i % 6 == 5) face++;
	}
	indices = new unsigned long[36];
	for (int i = 0; i < 36; ++i)
	{
		indices[i] = i;
	}

	vertexCount = 36;
	indexCount = 36;
	InitBuffers(device, vertices, indices);
}

void MeshComponent::MakeSphere(ID3D11Device* device, float radius, int subDivisions)
{
	vertices = new VertexConfig[(subDivisions * subDivisions) + 1];
	int vertex = 0;

	float hStep = XM_2PI / subDivisions;
	float vStep = XM_PI / subDivisions;

	vertices[vertex].position = { 0, radius, 0 };
	vertices[vertex].normal = { 0, 1, 0 };
	vertices[vertex].textureUV = { 0,0 };
	vertex++;
	for (int i = 1; i < subDivisions; ++i)
	{
		float vTheta = i * vStep;
		for (int j = 0; j <= subDivisions; ++j)
		{
			float hTheta = j * hStep;

			XMVECTOR position = { radius * XMScalarSin(vTheta) * XMScalarCos(hTheta),
							radius * XMScalarCos(vTheta),
							radius * XMScalarSin(vTheta) * XMScalarSin(hTheta)
			};

			XMVECTOR normal = XMVector3Normalize(position);
			XMFLOAT3 positionHolder;
			XMFLOAT3 normalHolder;

			XMStoreFloat3(&positionHolder, position);
			XMStoreFloat3(&normalHolder, normal);

			vertices[vertex].position = { positionHolder.x, positionHolder.y, positionHolder.z };
			vertices[vertex].normal = { normalHolder.x, normalHolder.y, normalHolder.z };

			float u, v;
			u = (float)j / subDivisions;
			v = (float)i / subDivisions;
			vertices[vertex].textureUV = { u, v };

			vertex++;
		}
	}
	vertices[vertex].position = { 0, -radius, 0 };
	vertices[vertex].normal = { 0, -1, 0 };
	vertices[vertex].textureUV = { 0, 1 };
	vertex++;

	indices = new unsigned long[6 * ((subDivisions * subDivisions) + subDivisions)];
	int index = 0;

	for (int i = 0; i <= subDivisions; ++i)
	{	indices[index++] = 0;
		indices[index++] = i + 1;
		indices[index++] = i + 2;
	}


	for (int i = 0; i < subDivisions; ++i)
	{
		for (int j = 0; j < subDivisions; ++j)
		{
			if (i == subDivisions - 1)
			{
				indices[index++] = vertex - 1;
				indices[index++] = (vertex - (subDivisions + 1)) + j;
				indices[index++] = (vertex - (subDivisions + 1)) + j + 1;
			}

			else
			{
				int  ringCount = subDivisions + 1;
				indices[index++] = 1 + (i * ringCount + j);
				indices[index++] = 1 + (i * ringCount + j + 1);
				indices[index++] = 1 + ((i + 1) * ringCount + j);

				indices[index++] = 1 + ((i + 1) * ringCount + j);
				indices[index++] = 1 + (i * ringCount + j + 1);
				indices[index++] = 1 + ((i + 1) * ringCount + j + 1);
			}
		}
	}

	vertexCount = vertex;
	indexCount = index;
	InitBuffers(device, vertices, indices);
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

VertexConfig* MeshComponent::GetVertices()
{
	return vertices;
}

unsigned long* MeshComponent::GetIndices()
{
	return indices;
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

bool MeshComponent::InitBuffers(ID3D11Device* device, VertexConfig* vertices, unsigned long* indices)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	if (!indices) return false;

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
	
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		MessageBox(hWnd, L"Failed to create index buffer", L"FAILED", MB_ICONERROR | MB_OK);
		return false;
	}

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

void MeshComponent::RotateMesh(const XMMATRIX& rotation)
{
	rotationMatrix = rotation;
}
