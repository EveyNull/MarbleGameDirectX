#include "CameraComponent.h"

CameraComponent::CameraComponent()
{
	camPos = { 0.0f, 0.0f, -10.0f };
	camRotX = 0.0f;
	camRotY = 0.0f;
	camRotZ = 0.0f;
	camUp = { 0.0f, 1.0f, 0.0f };
	camLocalPos = { -5.0f, 0.0f, 0.0f };

	perspMatrix = XMMatrixIdentity();
}


void CameraComponent::SetLocalPosition(XMMATRIX newPos)
{

}

void CameraComponent::SetPosition(XMVECTOR newPos)
{
	camPos = newPos;
}

XMVECTOR CameraComponent::GetPosition()
{
	return camPos;
}

void CameraComponent::SetRotation(VECTOR3 newRot)
{
	camRotX = newRot.x;
	camRotY = newRot.y;
	camRotZ = newRot.z;
}

VECTOR3 CameraComponent::GetRotation()
{
	return { camRotX, camRotY, camRotZ };
}

float* CameraComponent::GetYawPtr()
{
	return &camRotY;
}

bool CameraComponent::Render()
{
	XMVECTOR up, position, target;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix, translationMatrix;

	XMFLOAT3 camPosFloat3;
	XMStoreFloat3(&camPosFloat3, camPos);
	XMFLOAT3 offset = { -5.0f, 1.0f, 0.0f };


	pitch = camRotX * (XM_PI / 180);
	yaw = (camRotY + 90) * (XM_PI / 180);
	roll = camRotZ * (XM_PI / 180);

	up = { 0.0f, 1.0f, 0.0f };
	XMVECTOR newPos =
	{
		camPosFloat3.x - offset.x * (std::cos(yaw)),
		camPosFloat3.y + offset.y,
		camPosFloat3.z - offset.x * (std::sin(yaw))
	};

	position = newPos;
	target = camPos;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	translationMatrix = XMMatrixTranslation(-5.0f, 0.0f, 0.0f);

	//up = XMVector3TransformCoord(up, rotationMatrix);

	//position = XMVector3TransformCoord(position, translationMatrix);
	

	//target = XMVector3TransformCoord(position, rotationMatrix);

	//position += camPos;

	viewMatrix = XMMatrixLookAtLH(position, target, up);

	return true;
}

XMMATRIX* CameraComponent::GetViewMatrix()
{
	return &viewMatrix;
}

void CameraComponent::GetViewMatrix(XMMATRIX& matrix)
{
	matrix = viewMatrix;
}

XMMATRIX* CameraComponent::GetPerspMatrix()
{
	return &perspMatrix;
}
