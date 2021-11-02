#include "LightComponent.h"

LightComponent::LightComponent(XMVECTOR newDir, XMVECTOR newColor)
{
	direction = newDir;
	color = newColor;
	position = { 0.0f, 0.0f, 0.0f };
}

XMVECTOR LightComponent::GetColor()
{
	return color;
}

XMVECTOR LightComponent::GetDirection()
{
	return direction;
}