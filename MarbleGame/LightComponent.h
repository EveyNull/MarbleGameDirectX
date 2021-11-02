#pragma once
#include "Vector.h"

#include <DirectXMath.h>

using namespace DirectX;

class LightComponent
{
public:
	LightComponent(XMVECTOR, XMVECTOR);
	~LightComponent() = default;

	XMVECTOR GetDirection();
	XMVECTOR GetColor();
private:
	XMVECTOR direction;
	XMVECTOR position;
	XMVECTOR color;
};

