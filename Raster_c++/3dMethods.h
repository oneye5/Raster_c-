#pragma once
#define PI 3.1415926535897
#include <vector>
#include <cmath>
#include<fstream>
#include<strstream>
#include<string>

using std::vector;

struct Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
	Vector4(float X = 0, float Y = 0, float Z = 0,float W = 0)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
	friend Vector4 operator+(const Vector4& v1, const Vector4& v2)
	{
		float x = v1.x + v2.x;
		float y = v1.y + v2.y;
		float z = v1.z + v2.z;
		float w = v1.w + v2.w;
		return Vector4(x, y, z,w);
	}
	friend Vector4 operator-(const Vector4& v1, const Vector4& v2)
	{
		float x = v1.x - v2.x;
		float y = v1.y - v2.y;
		float z = v1.z - v2.z;
		float w = v1.w - v2.w;
		return Vector4(x, y, z,w);
	}
	friend Vector4 operator*(const Vector4& v1, const Vector4& v2)
	{
		float x = v1.x * v2.x;
		float y = v1.y * v2.y;
		float z = v1.z * v2.z;
		float w = v1.w * v2.w;
		return Vector4(x, y, z,w);
	}
	friend Vector4 operator*(const Vector4& v1, const float v2)
	{
		float x = v1.x * v2;
		float y = v1.y * v2;
		float z = v1.z * v2;
		float w = v1.w * v2;
		return Vector4(x, y, z,w);
	}
	friend Vector4 operator/(const Vector4& v1, const float v2)
	{
		float x = v1.x / v2;
		float y = v1.y / v2;
		float z = v1.z / v2;
		float w = v1.w / v2;
		return Vector4(x, y, z,w);
	}
};
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
	Vector3(Vector4 v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
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
	friend Vector3 operator/(const Vector3& v1, const float v2)
	{
		float x = v1.x / v2;
		float y = v1.y / v2;
		float z = v1.z / v2;
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
struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	Color(unsigned char R, unsigned char G, unsigned char B)
	{
		r = R;
		g = G;
		b = B;
	}
	Color() { 
		r = 255;
		g = 255;
		b = 255;
	};
};
struct Triangle
{
	vector< Vector4> verticies;
	Color col;
	Triangle(vector<Vector4> v)
	{
		verticies = v;
	}
	Triangle(float x,float y, float z, float x1, float y1, float z1,float x2, float y2,float z2)
	{
		verticies = vector<Vector4>{ Vector4(x,y,z,1.0f), Vector4(x1,y1,z1,1.0f),Vector4(x2,y2,z2,1.0f)};
	}
	Triangle()
	{
		verticies = vector<Vector4>{ Vector4(0,0,0), Vector4(0,0,0),Vector4(0,0,0) };
	}
	Triangle(Vector4 v1, Vector4 v2, Vector4 v3)
	{
		verticies = { v1,v2,v3 };
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
	bool loadFromObj(std::string dir)
	{
		std::ifstream f(dir);
		if (!f.is_open())
			return false;

		vector<Vector4> verticies;
		while (!f.eof())
		{
			char line[1000];
			f.getline(line, 1000);
			std::strstream str;
			str << line;
			char temp;
			if (line[0] == 'v')
			{
				Vector4 ver;
				str >> temp >> ver.x >> ver.y >> ver.z;
				ver.w = 1.0f;
				verticies.push_back(ver);
			}
			if (line[0] == 'f')
			{
				int f[3];
				str >> temp >> f[0] >> f[1] >> f[2];
				triangles.push_back(
					Triangle(
					verticies[f[0] - 1],
					verticies[f[1] - 1],
					verticies[f[2] - 1]
					)
				);
			
			}
		}
		return true;
	}
	Mesh() {};
};
struct Matrix4x4
{
	float m[4][4] = { 0 };
	friend Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
		return matrix;
	}
};
Vector4 multiplyMatrixVector( Matrix4x4& matrix,Vector4& input)
{
	Vector4 v;
	v.y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + input.w * matrix.m[3][1];
	v.z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + input.w * matrix.m[3][2];
	v.x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + input.w * matrix.m[3][0];
	v.w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + input.w * matrix.m[3][3];
	return v;
}
Vector4 multiplyMatrixVector(Matrix4x4& matrix ,Vector3& in)
{
	Vector4 v;
	Vector4 input = Vector4(in.x, in.y, in.z, 1.0f);
	v.y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + input.w * matrix.m[3][1];
	v.z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + input.w * matrix.m[3][2];
	v.x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + input.w * matrix.m[3][0];
	v.w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + input.w * matrix.m[3][3];
	return v;
}
Matrix4x4 getRotMatZ(float zRad)
{
	Matrix4x4 matrixRotZ;
	matrixRotZ.m[0][0] = cosf(zRad);
	matrixRotZ.m[0][1] = sinf(zRad);
	matrixRotZ.m[1][0] = -sinf(zRad);
	matrixRotZ.m[1][1] = cosf(zRad);
	matrixRotZ.m[2][2] = 1;
	matrixRotZ.m[3][3] = 1;
	return matrixRotZ;
}
Matrix4x4 getRotMatX(float xRad)
{
	Matrix4x4 matrixRotX;
	matrixRotX.m[0][0] = 1;
	matrixRotX.m[1][1] = cosf(xRad);
	matrixRotX.m[1][2] = sinf(xRad);
	matrixRotX.m[2][1] = -sinf(xRad);
	matrixRotX.m[2][2] = cosf(xRad);
	matrixRotX.m[3][3] = 1;
	return matrixRotX;
}
Matrix4x4 getProjectionMat(float fov, float aspectRatio,float farPlane, float nearPlane)
{
	Matrix4x4 projMatrix;
	float fovRad =  1.0f / tanf(degToRad(fov * 0.5));;
	projMatrix.m[0][0] = aspectRatio * fovRad;
	projMatrix.m[1][1] = fovRad;
	projMatrix.m[2][2] = farPlane / (farPlane - nearPlane);
	projMatrix.m[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
	projMatrix.m[2][3] = 1.0f;
	projMatrix.m[3][3] = 0.0f;
	return projMatrix;
}
Matrix4x4 getTranslationMat(Vector3 pos)
{
	Matrix4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	matrix.m[3][0] = pos.x;
	matrix.m[3][1] = pos.y;
	matrix.m[3][2] = pos.z;
	return matrix;
}
Matrix4x4 getEmptyMat()
{
	Matrix4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
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
void toNormalized(Vector3& v)
{
	float magnitude = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	if(magnitude != 0)
	v = Vector3(v.x / magnitude, v.y / magnitude, v.z / magnitude);
}
void toDotProduct(Vector3 a /*must be normals*/, Vector3 b, Vector3 camPos, float& out)
{
	out =
		a.x * (b.x - camPos.x) +
		a.y * (b.y - camPos.y) +
		a.z * (b.z - camPos.z);
}
float toDotProduct(Vector3 a /*must be normals*/, Vector3 b, Vector3 camPos)
{
	float	out =
		a.x * (b.x - camPos.x) +
		a.y * (b.y - camPos.y) +
		a.z * (b.z - camPos.z);
	return out;
}
float toDotProduct(Vector3 a /*must be normals*/, Vector3 b)
{
	float	out =
		a.x * (b.x) +
		a.y * (b.y) +
		a.z * (b.z);
	return out;
}
Vector3 crossProduct(Vector3 v1, Vector3 v2)
{
	Vector3 v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}
struct directionLight
{

	Vector3 dir;
	float lumens;
	Color color;
	directionLight() 
	{
		dir = Vector3(0, 0, 0);
		lumens = 0.0f;
		color = Color(255, 255, 255);
	};
};
float clamp(float in, float min, float max, bool Round)
{
	if (in < min)
		in = min;
	if (in > max)
		in = max;
	if (Round)
	{
		in = roundf(in);
	}
	return in;
}
Vector3 averagePos(Triangle t)
{
	Vector3 out;
	for (auto v : t.verticies)
	{
		out = out + v;
	}
	return out / 3.0f;
}
float magnitude(Vector3 v)
{
	return sqrtf(toDotProduct(v, v));
}
void devXYbyZ(Triangle& tri)
{
	for (auto& v : tri.verticies)
	{
		float d = v.z;
		v.x = v.x / d;
		v.y = v.y / d;
	}
}