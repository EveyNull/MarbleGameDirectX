#pragma once
#include "Vector.h"
#include <DirectXMath.h>

using namespace DirectX;

class CameraComponent
{
public:
	CameraComponent();
	~CameraComponent() = default;

	void SetLocalPosition(XMMATRIX);
	void SetPosition(XMVECTOR);

	XMVECTOR GetPosition();
	
	void SetRotation(VECTOR3);
	VECTOR3 GetRotation();

	float* GetYawPtr();

	void Update(float dt);
	bool Render();

	XMMATRIX* GetViewMatrix();
	void GetViewMatrix(XMMATRIX&);
	XMMATRIX* GetPerspMatrix();

private:
	XMVECTOR camPos;
	XMVECTOR camUp;
	XMVECTOR camLocalPos;
	XMMATRIX viewMatrix;
	XMMATRIX perspMatrix;

	float camRotX, camRotY, camRotZ;

	float camera_fov;
	float window_width;
	float window_height;
	float near_plane;
	float far_plane;
};

