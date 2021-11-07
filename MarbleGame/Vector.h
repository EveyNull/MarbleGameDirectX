#pragma once
#include <cmath>
struct VECTOR2
{
	float x;
	float y;
};

struct VECTOR3
{
	float x;
	float y;
	float z;
	void operator+=(const VECTOR3& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
	}
	void operator*=(const float& multi)
	{
		x *= multi;
		y *= multi;
		z *= multi;
	}
	VECTOR3 operator*(const VECTOR3& vector)
	{
		VECTOR3 output =
		{
			x * vector.x,
			y * vector.y,
			z * vector.z
		};
		return output;
	}
	VECTOR3 operator*(const float& multi)
	{
		VECTOR3 output = { x, y, z };
		output *= multi;
		return output;
	}
	VECTOR3 operator-()
	{
		return { -x, -y, -z };
	}
	VECTOR3 operator+(const VECTOR3& vector)
	{
		return { x + vector.x, y + vector.y, z + vector.z };
	}
	VECTOR3 operator-(const VECTOR3& vector)
	{
		return { x - vector.x, y - vector.y, z - vector.z };
	}
	bool operator>(float compare)
	{
		return abs(x) >= compare || abs(y) >= compare || abs(z) >= compare;
	}
	VECTOR3 absolute()
	{
		return { abs(x), abs(y), abs(z) };
	}
};

struct VECTOR4
{
	float x;
	float y;
	float z;
	float w;
};