#include "MathHelper.h"

float MathHelper::Dot(const VECTOR3& a, const VECTOR3& b)
{
	float output;
	XMVECTOR v1 = { a.x, a.y, a.z };
	XMVECTOR v2 = { b.x, b.y, b.z };
	XMStoreFloat(&output, XMVector3Dot(v1, v2));
	return output;
}

float MathHelper::Lerp(const float& a, const float& b, const float& t)
{
	return a + t * (b - a);
}

//Return number of bits required for biggest number in the level map data
int MathHelper::NumberOfBits(int input)
{
	int bits = 1;
	while (input > 1)
	{
		input /= 2;
		bits++;
	}
	return bits;
}

void MathHelper::BinarySeparateInts(int* ints, int input, int bitNumber)
{
	for (int i = bitNumber - 1; i >= 0; --i)
	{
		ints[i] = input % 2;
		input /= 2;
	}
}

void MathHelper::ClampValue(float& input, float min, float max)
{
	if (input < min)
	{
		input = min;
		return;
	}
	if (input > max)
	{
		input = max;
		return;
	}
}

void MathHelper::MinValue(float& input, float min)
{
	if (input < min) input = min;
}
void MathHelper::MaxValue(float& input, float max)
{
	if (input > max) input = max;
}