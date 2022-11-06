#pragma once
#define PI 3.1415926535897
#include <vector>
#include <cmath>
#include<fstream>
#include<strstream>
#include<string>
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro.h"
#include "allegro5/bitmap.h"
#include"allegro5/allegro_image.h"
#pragma warning(disable : 4996)
using std::vector;


struct Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
	Vector4(float X = 0, float Y = 0, float Z = 0,float W = 1.0f)
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
	vector3(Vector4 v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
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
	vector2(Vector4 v)
	{
		x = v.x;
		y = v.y;
	}
	vector2(vector3 v)
	{
		x = v.x;
		y = v.y;
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
	Color(float R, float G, float B)
	{
		r = (int)R;
		g = (int)G;
		b = (int)B;
	}
	Color() { 
		r = 255;
		g = 255;
		b = 255;
	};
	Color(ALLEGRO_COLOR c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
	}
};
struct triColInfo
{
	Color col;
};
struct Triangle
{
	vector< Vector4> verticies;
	triColInfo colInfo;
	vector<vector2> textureCoords = vector<vector2>{vector2(),vector2() ,vector2() };
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
	vector3 pos;
	vector3 rot;
	vector<Triangle> triangles;
	Mesh(vector<Triangle> tris,vector3 position = vector3(0,0,2) ,vector3 rotation = vector3(0,0,0))
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
			char line[10000];
			f.getline(line, 10000);
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

			std::cout << "loading line "<<verticies.size()<< "\n";
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
Vector4 multiplyMatrixVector( Matrix4x4& matrix,Vector4& Input)
{
	Vector4 v;
	v.y = Input.x * matrix.m[0][1] + Input.y * matrix.m[1][1] + Input.z * matrix.m[2][1] + Input.w * matrix.m[3][1];
	v.z = Input.x * matrix.m[0][2] + Input.y * matrix.m[1][2] + Input.z * matrix.m[2][2] + Input.w * matrix.m[3][2];
	v.x = Input.x * matrix.m[0][0] + Input.y * matrix.m[1][0] + Input.z * matrix.m[2][0] + Input.w * matrix.m[3][0];
	v.w = Input.x * matrix.m[0][3] + Input.y * matrix.m[1][3] + Input.z * matrix.m[2][3] + Input.w * matrix.m[3][3];
	return v;
}
Vector4 multiplyMatrixVector(Matrix4x4& matrix ,vector3& in)
{
	Vector4 v;
	Vector4 Input = Vector4(in.x, in.y, in.z, 1.0f);
	v.y = Input.x * matrix.m[0][1] + Input.y * matrix.m[1][1] + Input.z * matrix.m[2][1] + Input.w * matrix.m[3][1];
	v.z = Input.x * matrix.m[0][2] + Input.y * matrix.m[1][2] + Input.z * matrix.m[2][2] + Input.w * matrix.m[3][2];
	v.x = Input.x * matrix.m[0][0] + Input.y * matrix.m[1][0] + Input.z * matrix.m[2][0] + Input.w * matrix.m[3][0];
	v.w = Input.x * matrix.m[0][3] + Input.y * matrix.m[1][3] + Input.z * matrix.m[2][3] + Input.w * matrix.m[3][3];
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
Matrix4x4 getRotMatY(float yRad)
{
	Matrix4x4 matrix;
	matrix.m[0][0] = cosf(yRad);
	matrix.m[0][2] = sinf(yRad);
	matrix.m[2][0] = -sinf(yRad);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosf(yRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
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
Matrix4x4 getTranslationMat(vector3 pos)
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
	auto pos = vector2
	(
		(x + 1.0f) * ((float)w / 2.0f),
		(y + 1.0f) * ((float)h / 2.0f)
	);

	pos = vector2(roundf(pos.x), roundf(pos.y));
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
void toNormalized(vector3& v)
{
	float magnitude = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	if(magnitude != 0)
	v = vector3(v.x / magnitude, v.y / magnitude, v.z / magnitude);
}
void toDotProduct(vector3 a /*must be normals*/, vector3 b, vector3 camPos, float& out)
{
	out =
		a.x * (b.x - camPos.x) +
		a.y * (b.y - camPos.y) +
		a.z * (b.z - camPos.z);
}
float toDotProduct(vector3 a /*must be normals*/, vector3 b, vector3 camPos)
{
	float	out =
		a.x * (b.x - camPos.x) +
		a.y * (b.y - camPos.y) +
		a.z * (b.z - camPos.z);
	return out;
}
float toDotProduct(vector3 a /*must be normals*/, vector3 b)
{
	float	out =
		a.x * (b.x) +
		a.y * (b.y) +
		a.z * (b.z);
	return out;
}
vector3 crossProduct(vector3 v1, vector3 v2)
{
	vector3 v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

struct directionLight
{

	vector3 dir;
	float lumens;
	float ambientMulti;
	Color color;
	directionLight() 
	{
		dir = vector3(0, 0, 0);
		lumens = 0.0f;
		color = Color((unsigned char)255, (unsigned char)255, (unsigned char)255);
		ambientMulti = 0.0f;
	};
};
struct pointLight
{
	float lumens;
	float range;
	Color color;
	vector3 pos;
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
vector3 averagePos(Triangle t)
{
	vector3 out;
	for (auto v : t.verticies)
	{
		out = out + v;
	}
	return out / 3.0f;
}
float magnitude(vector3 v)
{
	return sqrtf(toDotProduct(v, v));
}
Matrix4x4 matPointAt(vector3 pos, vector3 target, vector3 up)
{
	vector3 newForward = target - pos;
	toNormalized(newForward);

	// new up 

	vector3 v = newForward * toDotProduct(up, newForward);
	vector3 newUp = up - v;
	toNormalized(newUp);

	//right
	vector3 newRight = crossProduct(newUp, newForward);

	Matrix4x4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
		return matrix;
}
Matrix4x4 matInverse(Matrix4x4& m)
{
	Matrix4x4 matrix;
	matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
vector3 rayFromAngle(vector2 A)
{
	vector3 pos
	(
		sinf(A.y) * cosf(A.x),
		cosf(A.x),
		cosf(A.y) * cosf(A.x)
	);
	return pos;
}
vector3 intersectPlane(vector3& plane_p, vector3& plane_n, vector3& lineStart, vector3& lineEnd,float& t)
{
	toNormalized(plane_n);
	float plane_d = -toDotProduct(plane_n, plane_p);
	float ad = toDotProduct(lineStart, plane_n);
	float bd = toDotProduct(lineEnd, plane_n);
	 t = (-plane_d - ad) / (bd - ad);
	vector3 lineStartToEnd = lineEnd - lineStart;
	vector3 lineToIntersect = lineStartToEnd * t;
	return lineStart + lineToIntersect;
}
vector3 intersectPlane(vector3& plane_p, vector3& plane_n, Vector4& lineStart4, Vector4& lineEnd4,float& t)
{
	auto lineStart = vector3(lineStart4);
	auto lineEnd = vector3(lineEnd4);
	toNormalized(plane_n);
	float plane_d = -toDotProduct(plane_n, plane_p);
	float ad = toDotProduct(lineStart, plane_n);
	float bd = toDotProduct(lineEnd, plane_n);
	t = (-plane_d - ad) / (bd - ad);
	vector3 lineStartToEnd = lineEnd - lineStart;
	vector3 lineToIntersect = lineStartToEnd * t;
	return lineStart + lineToIntersect;
}
float dist(Vector4& p, vector3 plane_n,vector3 plane_p)
{
	vector3 n = vector3(p);
	toNormalized(n);
	return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - toDotProduct(plane_n, plane_p));
}
int TriClipFromPlane(vector3 plane_p, vector3 plane_n, Triangle& in_tri, Triangle& out_tri1, Triangle& out_tri2)
{
	// Make sure plane normal is indeed normal
	toNormalized(plane_n);

	// Create two temporary storage arrays to classify points either side of plane
	// If distance sign is positive, point lies on "inside" of plane
	Vector4* inside_points[3];  int nInsidePointCount = 0;
	Vector4* outside_points[3]; int nOutsidePointCount = 0;

	Vector4* inside_texture[3]; int insideTexCount = 0;
	Vector4* outside_texture[3]; int outsideTexCount = 0;
	// Get signed distance of each point in triangle to plane
	float d0 = dist(in_tri.verticies[0],plane_n,plane_p);
	float d1 = dist(in_tri.verticies[1],plane_n,plane_p);
	float d2 = dist(in_tri.verticies[2],plane_n,plane_p);

	if (d0 >= 0) 
	{
		inside_points[nInsidePointCount] = &in_tri.verticies[0];
		inside_texture[insideTexCount] = &in_tri.verticies[0];
		insideTexCount++;
		nInsidePointCount++;
		
	}
	else 
	{
		outside_points[nOutsidePointCount] = &in_tri.verticies[0];
		outside_texture[outsideTexCount] = &in_tri.verticies[0];
		outsideTexCount++;
		nOutsidePointCount++;
	}

	if (d1 >= 0)
	{ 
		inside_points[nInsidePointCount] = &in_tri.verticies[1];
		inside_texture[insideTexCount] = &in_tri.verticies[1];
		insideTexCount++;
		nInsidePointCount++;
	}
	else 
	{
		outside_points[nOutsidePointCount] = &in_tri.verticies[1];
		outside_texture[outsideTexCount] = &in_tri.verticies[1];
		outsideTexCount++;
		nOutsidePointCount++;
	}

	if (d2 >= 0)
	{ 
		inside_points[nInsidePointCount] = &in_tri.verticies[2];
		inside_texture[insideTexCount] = &in_tri.verticies[2];
		insideTexCount++;
		nInsidePointCount++;
	}
	else 
	{
		outside_points[nOutsidePointCount] = &in_tri.verticies[2];
		outside_texture[outsideTexCount] = &in_tri.verticies[2];
		outsideTexCount++;
		nOutsidePointCount++;
	}

	// Now classify triangle points, and break the input triangle into 
	// smaller output triangles if required. There are four possible
	// outcomes...

	if (nInsidePointCount == 0)
	{
		// All points lie on the outside of plane, so clip whole triangle
		// It ceases to exist

		return 0; // No returned triangles are valid
	}

	if (nInsidePointCount == 3)
	{
		// All points lie on the inside of plane, so do nothing
		// and allow the triangle to simply pass through
		out_tri1 = in_tri;

		return 1; // Just the one returned original triangle is valid
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		// Triangle should be clipped. As two points lie outside
		// the plane, the triangle simply becomes a smaller triangle

		// Copy appearance info to new triangle
		out_tri1.colInfo = in_tri.colInfo;


		// The inside point is valid, so keep that...
		out_tri1.verticies[0] = *inside_points[0];
		out_tri1.textureCoords[0] = vector2(* inside_texture[0]);
		// but the two new points are at the locations where the 
		// original sides of the triangle (lines) intersect with the plane
	
		float t;
		auto temp1 = intersectPlane(plane_p, plane_n,  *inside_points[0], *outside_points[0],t) ;
		out_tri1.textureCoords[1] = t * (outside_points[0]->x - inside_texture[0]->x  ) + inside_texture[0]->x;
		out_tri1.textureCoords[1] = t * (outside_points[0]->y - inside_texture[0]->y  ) + inside_texture[0]->y;

		auto temp2 = intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1],t);
		out_tri1.textureCoords[2] = t * (outside_points[0]->x - inside_texture[0]->x) + inside_texture[0]->x;
		out_tri1.textureCoords[2] = t * (outside_points[0]->y - inside_texture[0]->y) + inside_texture[0]->y;

		out_tri1.verticies[1] = Vector4( temp1.x,temp1.y,temp1.z);
		out_tri1.verticies[2] = Vector4(temp2.x, temp2.y, temp2.z);

		return 1; // Return the newly formed single triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		// Copy appearance info to new triangles
		out_tri1.colInfo = in_tri.colInfo;
		out_tri2.colInfo = in_tri.colInfo;

		// The first triangle consists of the two inside points and a new
		// point determined by the location where one side of the triangle
		// intersects with the plane
		out_tri1.verticies[0] = *inside_points[0];
		out_tri1.verticies[1] = *inside_points[1];
		out_tri1.textureCoords[0]  = vector2(*inside_texture[0]);
		out_tri1.textureCoords[1] = vector2(*inside_texture[1]);

		float t;
		 auto temp1 = intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0],t);
		 out_tri1.verticies[2] = Vector4(temp1.x, temp1.y, temp1.z);
		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		out_tri2.verticies[0] = *inside_points[1];
		out_tri2.verticies[1] = out_tri1.verticies[2];


		auto temp2 = intersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0],t);
		out_tri2.verticies[2] = Vector4(temp2.x,temp2.y,temp2.z);
		return 2; // Return two newly formed triangles which form a quad
	}
}
Color sampleTexture(vector2 v, ALLEGRO_BITMAP* t)
{
	int w=	al_get_bitmap_width(t);
	int h = al_get_bitmap_height(t);

	normToScreen(w, h, v.x, v.y);
	int x = v.x;
	int y = v.y;

	return	al_get_pixel(t, x, y);
}
void textureTri(			//textureing not implimented due to per pixel rendering being too slow, this is likley because of the heavy cpu bias in this program
	int x1, int y1, float u1, float v1,
	int x2, int y2, float u2, float v2,
	int x3, int y3, float u3, float v3,ALLEGRO_BITMAP* texture)
{
	if (y2 < y1)
	{
		std::swap(y1, y2);
		std::swap(x1, x2);
		std::swap(u1, u2);
		std::swap(v1, v2);
	}
	if (y3 < y1)
	{
		std::swap(y1, y3);
		std::swap(x1, x3);
		std::swap(u1, u3);
		std::swap(v1, v3);
	}
	if (y3 < y2)
	{
		std::swap(y2, y3);
		std::swap(x2, x3);
		std::swap(u2, u3);
		std::swap(v2, v3);
	}

	int dy1 = y2 - y1;
	int dx1 = x2 - x1;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;

	float texU, texV;

	float daxStep = 0.0f;
	float dbxStep = 0.0f;
	float du1Step = 0.0f;
	float dv1Step = 0.0f;
	float du2Step = 0.0f;
	float dv2Step = 0.0f;

	if (dy1) daxStep = dx1 / (float)abs(dy1);
	if (dy2) dbxStep = dx2 / (float)abs(dy2);


	if (dy1) du1Step = du1 / (float)abs(dy1);
	if (dy1) dv1Step = dx1 / (float)abs(dy1);

	if (dy2) du2Step = du2 / (float)abs(dy2);
	if (dy2) dv2Step = dx2 / (float)abs(dy2);

	if (dy1)
	{
		for (int y = y1; y <= y2; y++)
		{
			int ax = x1 + (float)(y - y1) * daxStep;
			int bx = x1 + (float)(y - y1) * dbxStep;

			float texSu = u1 + (float)(y - y1) * du1Step; //starting values
			float texSv = v1 + (float)(y - y1) * dv1Step;

			float texEu = u1 + (float)(y - y1) * du2Step;
			float texEv = v1 + (float)(y - y1) * du2Step;//ending values

			if (ax > bx)
			{
				std::swap(ax, bx);
				std::swap(texSu, texEu);
				std::swap(texSv, texEv);
			}

			texU = texSu;
			texV = texSv;

			float tStep = 1.0f / ((float)(bx - ax));
			float t = 0.0f; // where

			for (int x = ax; x < bx; x++)
			{
				texU = (1.0f - t) * texSu + t * texEu;
				texV = (1.0f - t) * texSu + t * texEv;

				

				//draw

				auto c = sampleTexture(vector2(texU, texEv),texture);
				al_draw_pixel(x, y, al_map_rgb(c.r, c.g, c.b));


				t += tStep;
			}
		}
		// bottom tri
			
		dy1 = y3 - y2;
		dx1 = x3 - x2;
		dv1 = v3 - v2;
		du1 = u3 - u2;

		if (dy1) daxStep = dx1 / (float)abs(dy1);
		if(dy2) dbxStep = dx2 / (float)abs(dy2);

		du1Step = 0.0f;
		dv1Step = 0.0f;

		if (dy1) du1Step = du1 / (float)abs(dy1);
		if (dy2) dv1Step = dv1 / (float)abs(dy2);



		for (int y = y2; y <= y3; y++)
		{
			int ax = x2 + (float)(y - y2) * daxStep;
			int bx = x1 + (float)(y - y1) * dbxStep;

			float texSu = u2 + (float)(y - y2) * du1Step; //starting values
			float texSv = v2 + (float)(y - y2) * dv1Step;

			float texEu = u1 + (float)(y - y1) * du2Step;
			float texEv = v1 + (float)(y - y1) * du2Step;//ending values

			if (ax > bx)
			{
				std::swap(ax, bx);
				std::swap(texSu, texEu);
				std::swap(texSv, texEv);
			}

			texU = texSu;
			texV = texSv;

			float tStep = 1.0f / ((float)(bx - ax));
			float t = 0.0f; // where

			for (int x = ax; x < bx; x++)
			{
				texU = (1.0f - t) * texSu + t * texEu;
				texV = (1.0f - t) * texSu + t * texEv;



				//draw

			//	auto c = sampleTexture(Vector2(texU, texEv), texture);
				//al_draw_pixel(x, y, al_map_rgb(c.r, c.g, c.b));
				al_draw_pixel(x, y, al_map_rgb(255, 255, 255));

				t += tStep;
			}
		}
	}
}	


void degToRad(vector3& v)
{
	v = vector3(degToRad(v.x), degToRad(v.y), radToDeg(v.z));
}

float getDistance(vector3 v1, vector3 v2)
{
	auto out =	v1 - v2;
	return	magnitude(out);
}