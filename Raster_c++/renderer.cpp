#include<iostream>
#include"renderer.h"
int screenWidth;
int screenHeight;

const float nearPlane = 0.1f;
const float farPlane = 1000.0f;
const float fov = 90.0f;
float aspectRatio;
float fovRad = degToRad(1.0f / tanf(fov * 0.5));

Matrix4x4 projMatrix;

void ViewPort::InitViewPort(int ScreenWidth, int ScreenHeight)
{
	screenWidth = ScreenWidth;
	screenHeight = ScreenHeight;
	aspectRatio = (float)screenHeight / (float)screenWidth;

	projMatrix.m[0][0] = aspectRatio * fovRad;
	projMatrix.m[1][1] = fovRad;
	projMatrix.m[2][2] = farPlane / (farPlane - nearPlane);
	projMatrix.m[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
	projMatrix.m[2][3] = 1.0f;
	projMatrix.m[3][3] = 0.0f;
}



void ViewPort::InitGeometry()
{
}

void drawGeometry(vector<Mesh> geometry)
{
	for (auto obj : geometry)
	{
		for (auto tri : obj.triangles)
		{
			Triangle projected;
			multiplyMatrixVector(tri.verticies[0], projected.verticies[0], projMatrix);
			multiplyMatrixVector(tri.verticies[1], projected.verticies[1], projMatrix);
			multiplyMatrixVector(tri.verticies[2], projected.verticies[2], projMatrix);
		}
	}
}
