#pragma once
#include "Vector.h"
#include <DirectXMath.h>

using namespace DirectX;

class MathHelper
{
public:
	static float Dot(const VECTOR3&, const VECTOR3&);
	static float Lerp(const float& a, const float& b, const float& t);
	static int NumberOfBits(int input);
	static void BinarySeparateInts(int* ints, int input, int bitNumber);
	static void ClampValue(float& input, float min, float max);
	static void MinValue(float& input, float min);
	static void MaxValue(float& input, float max);
};

