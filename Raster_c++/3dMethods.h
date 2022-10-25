#pragma once
#define PI 3.1415926535897
#include <vector>
#include <cmath>
using std::vector;

struct Vector3
{
public:
	float x;
	float y;
	float z;
	Vector3(float X = 0, float Y = 0, float Z = 0)
	{
		x = X;
		y = Y;
		z = Z;
	}
	friend Vector3 operator+(const Vector3& v1, const Vector3& v2)
	{
		float x = v1.x + v2.x;
		float y = v1.y + v2.y;
		float z = v1.z + v2.z;
		return Vector3(x, y, z);
	}
	friend Vector3 operator-(const Vector3& v1, const Vector3& v2)
	{
		float x = v1.x - v2.x;
		float y = v1.y - v2.y;
		float z = v1.z - v2.z;
		return Vector3(x, y, z);
	}
	friend Vector3 operator*(const Vector3& v1, const Vector3& v2)
	{
		float x = v1.x * v2.x;
		float y = v1.y * v2.y;
		float z = v1.z * v2.z;
		return Vector3(x, y, z);
	}
	friend Vector3 operator*(const Vector3& v1, const float v2)
	{
		float x = v1.x * v2;
		float y = v1.y * v2;
		float z = v1.z * v2;
		return Vector3(x, y, z);
	}
};
struct Vector2
{
public:
	float x;
	float y;
	Vector2(float X = 0, float Y = 0)
	{
		x = X;
		y = Y;
	}
};
float radToDeg(float rad)
{
	return(rad * (180 / PI));
}
float degToRad(float deg)
{
	return(deg * (PI / 180));
}

struct Triangle
{
	vector< Vector3> verticies;
	Triangle(vector<Vector3> v)
	{
		verticies = v;
	}
	Triangle(float x,float y, float z, float x1, float y1, float z1,float x2, float y2,float z2)
	{
		verticies = vector<Vector3>{ Vector3(x,y,z), Vector3(x1,y1,z1),Vector3(x2,y2,z2)};
	}
	Triangle()
	{
		verticies = vector<Vector3>{ Vector3(0,0,0), Vector3(0,0,0),Vector3(0,0,0) };
	}
};
struct Mesh
{
	Vector3 pos;
	Vector3 rot;
	vector<Triangle> triangles;
	Mesh(vector<Triangle> tris,Vector3 position = Vector3(0,0,2) ,Vector3 rotation = Vector3(0,0,0))
	{
		pos = position;
		rot = rotation;
		triangles = tris;
	}

};
Mesh cubeMesh()
{
	#pragma region meshDef
	Mesh m = Mesh
	(
		vector<Triangle>
	{
		//front face
		Triangle
		(
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		)
			,
			Triangle
			(
				0.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f
			)
			,

			// right face
			Triangle
			(
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 0.0f
			)
			,
			Triangle
			(
				1.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f
			)
			//back face
			,
			Triangle
			(
				1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f
			)
			,
			Triangle
			(
				0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f
			)
			//left face
			,
			Triangle
			(
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 1.0f
			)
			,
			Triangle
			(
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 1.0f
			)
			//top face
			,
			Triangle
			(
				0.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 1.0f
			)
			,
			Triangle
			(
				1.0f, 1.0f, 1.0f,
				0.0f, 1.0f, 1.0f,
				0.0f, 1.0f, 0.0f
			)
			//bottom face
			,
			Triangle
			(
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f
			)
			,
			Triangle
			(
				0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f
			)
	}
	);
#pragma endregion
	return m;

}
struct Matrix4x4
{
	float m[4][4] = { 0 };
};
void multiplyMatrixVector(Vector3& input, Vector3& output, Matrix4x4& matrix)
{
	output.x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + matrix.m[3][0];
	output.y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + matrix.m[3][1];
	output.z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + matrix.m[3][2];
	float w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + matrix.m[3][3];

	if (w != 0.0f)
	{
		output.x /= w;
		output.y /= w;
		output.z /= w;
	}
}
void normToScreen(int w, int h, float& x,float& y)
{
	auto pos = Vector2
	(
		(x + 1.0f) * ((float)w / 2.0f),
		(y + 1.0f) * ((float)h / 2.0f)
	);

	pos = Vector2(roundf(pos.x), roundf(pos.y));
	x = pos.x;
	y = pos.y;
}
void normToScreen(int w, int h,Triangle& t)
{
	for (auto& v : t.verticies)
	{
		normToScreen(w, h, v.x, v.y);
	}
}
Mesh translate(Vector3 v, Mesh m)
{
	auto& t = m.triangles;
	for (auto& tri : t)
	{
		for (auto& ver : tri.verticies)
		{
			ver = v + ver;
		}
	}
	return m;
}
