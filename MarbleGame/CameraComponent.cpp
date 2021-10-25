#include "CameraComponent.h"

CameraComponent::CameraComponent()
{
	camPos = { 0.0f, 0.0f, -10.0f };
	camRotX = 0.0f;
	camRotY = 0.0f;
	camRotZ = 0.0f;
	camUp = { 0.0f, 1.0f, 0.0f };


	perspMatrix = XMMatrixIdentity();
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

bool CameraComponent::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up = { 0.0f, 1.0f, 0.0f };

	// Setup the position of the camera in the world.
	position = camPos;

	// Setup where the camera is looking by default.
	lookAt = { 0.0f, 0.0f, 0.1f };

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = camRotX * 0.0174532925f;
	yaw = camRotY * 0.0174532925f;
	roll = camRotZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

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
