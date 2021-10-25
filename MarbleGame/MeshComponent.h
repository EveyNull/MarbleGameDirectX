#pragma once

#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "VertexConfig.h"

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class MeshComponent
{
public:
	MeshComponent(HWND& hWnd);
	~MeshComponent();
	void MakeTriangle(ID3D11Device* device);
	void MakeCube(ID3D11Device* device);
	void Render(ID3D11DeviceContext*);

	void TranslateMesh(VECTOR3 position);
	void RotateMesh(VECTOR3 rotation);

	XMMATRIX GetRotationMatrix();
	XMMATRIX GetTranslationMatrix();

	int GetIndexCount();

private:
	bool InitBuffers(ID3D11Device*, VertexConfig*, int);
	void RenderBuffers(ID3D11DeviceContext*);

	HWND hWnd;

	XMMATRIX rotationMatrix;
	XMMATRIX translationMatrix;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount, indexCount;
};

#endif