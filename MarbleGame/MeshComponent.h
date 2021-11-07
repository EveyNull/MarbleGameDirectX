#pragma once

#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Texture.h"
#include "VertexConfig.h"

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class MeshComponent
{
public:
	MeshComponent() = default;
	MeshComponent(HWND& hWnd);
	~MeshComponent();
	void MakeCube(ID3D11Device*, float, float, float);
	void MakeSphere(ID3D11Device*, float, int);

	void Render(ID3D11DeviceContext*);

	void TranslateMesh(VECTOR3 position);
	void RotateMesh(VECTOR3 rotation);

	void LoadTexture(ID3D11Device*, const wchar_t*);
	ID3D11ShaderResourceView* GetTexture();

	VertexConfig* GetVertices();
	unsigned long* GetIndices();

	XMMATRIX GetRotationMatrix();
	XMMATRIX GetTranslationMatrix();

	int GetIndexCount();

private:
	bool InitBuffers(ID3D11Device*, VertexConfig*, unsigned long*);
	void RenderBuffers(ID3D11DeviceContext*);

	HWND hWnd;

	XMMATRIX rotationMatrix;
	XMMATRIX translationMatrix;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	Texture* texture;

	VertexConfig* vertices;
	unsigned long* indices;
	int vertexCount, indexCount;
};

#endif