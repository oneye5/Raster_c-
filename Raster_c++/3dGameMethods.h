#include <windows.h>  
#include <iostream>
#define PI 3.1415926535897



struct vector3
{
public:
	float x;
	float y;
	float z;
	vector3(float X = 0, float Y = 0, float Z = 0)
	{
		x = X;
		y = Y;
		z = Z;
	}

	friend vector3 operator+(const vector3& v1, const vector3& v2)
	{
		float x = v1.x + v2.x;
		float y = v1.y + v2.y;
		float z = v1.z + v2.z;
		return vector3(x, y, z);
	}
	friend vector3 operator-(const vector3& v1, const vector3& v2)
	{
		float x = v1.x - v2.x;
		float y = v1.y - v2.y;
		float z = v1.z - v2.z;
		return vector3(x, y, z);
	}
	friend vector3 operator*(const vector3& v1, const vector3& v2)
	{
		float x = v1.x * v2.x;
		float y = v1.y * v2.y;
		float z = v1.z * v2.z;
		return vector3(x, y, z);
	}
	friend vector3 operator*(const vector3& v1, const float v2)
	{
		float x = v1.x * v2;
		float y = v1.y * v2;
		float z = v1.z * v2;
		return vector3(x, y, z);
	}
	friend vector3 operator/(const vector3& v1, const float v2)
	{
		float x = v1.x / v2;
		float y = v1.y / v2;
		float z = v1.z / v2;
		return vector3(x, y, z);
	}
};
struct vector2
{
public:
	float x;
	float y;
	vector2(float X = 0, float Y = 0)
	{
		x = X;
		y = Y;
	}
	vector2(vector3 v)
	{
		x = v.x;
		y = v.y;
	}
};
double degToRad(float deg)
{
	return(deg * (PI / 180));
}


vector3 rayFromAngle(vector2 A,float nothing = 0.0f)
{
	vector3 pos = vector3(
		-sinf(degToRad(A.y)),
		0.0f,
		cosf(degToRad(A.y))
		);
	return pos;
}

vector3 camForward(vector3 v)
{
	vector3 pos = rayFromAngle(v);
	return pos;
}
vector3 camRight(vector3 v)
{
	v.y = v.y + 90.0f;
	vector3 pos = rayFromAngle(v);

	return pos;
}
void clipRot(vector3& v)
{
	if (v.x > 90.0f)
		v.x = 90.0f;
	if (v.x < -90.0f)
		v.x = -90.0f;

	if (v.y > 180.0f)
		v.y = v.y - 360.0f;
	if (v.y < -180.0f)
		v.y= v.y + 360.0f;

	if (v.z > 180.0f)
		v.z =v.z - 360.0f;
	if (v.z < -180.0f)
		v.z = v.z + 360.0f;

	std::cout << v.x << " " << v.y << "\n";
}
