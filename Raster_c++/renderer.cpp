#include<iostream>
#include"renderer.h"
#include"3dMethods.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro.h"
int screenWidth;
int screenHeight;

const float nearPlane = 0.0f;
const float farPlane = 1000.0f;
const float fov = 0.02f;
float aspectRatio;
float fovRad = degToRad(1.0f / tanf(fov * 0.5));

Matrix4x4 projMatrix;
 vector<Mesh>geometry;

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
	geometry = vector<Mesh>{ cubeMesh()};
	geometry[0].pos = Vector3(2, 0, 5);
	geometry[0].rot = Vector3(1, 4, 6);
}

void ViewPort::render()
{
	for (auto obj : geometry)
	{
		//================================================= rotate =====================================
		Matrix4x4 matrixRotZ, matrixRotX;
		// Rotation Z
		matrixRotZ.m[0][0] = cosf(obj.rot.z);
		matrixRotZ.m[0][1] = sinf(obj.rot.z);
		matrixRotZ.m[1][0] = -sinf(obj.rot.z);
		matrixRotZ.m[1][1] = cosf(obj.rot.z);
		matrixRotZ.m[2][2] = 1;
		matrixRotZ.m[3][3] = 1;

		// Rotation X
		matrixRotX.m[0][0] = 1;
		matrixRotX.m[1][1] = cosf(obj.rot.x);
		matrixRotX.m[1][2] = sinf(obj.rot.x);
		matrixRotX.m[2][1] = -sinf(obj.rot.x);
		matrixRotX.m[2][2] = cosf(obj.rot.x);
		matrixRotX.m[3][3] = 1;

		for (auto tri : obj.triangles)
		{
			Triangle projected, translated, rotatedZ, rotated;
			//mat multi format is **IN . OUT , M
			multiplyMatrixVector(tri.verticies[0], rotatedZ.verticies[0], matrixRotZ);
			multiplyMatrixVector(tri.verticies[1], rotatedZ.verticies[1], matrixRotZ);
			multiplyMatrixVector(tri.verticies[2], rotatedZ.verticies[2], matrixRotZ);

			multiplyMatrixVector(rotatedZ.verticies[0], rotated.verticies[0], matrixRotX);
			multiplyMatrixVector(rotatedZ.verticies[1], rotated.verticies[1], matrixRotX);
			multiplyMatrixVector(rotatedZ.verticies[2], rotated.verticies[2], matrixRotX);
			translated = rotated;
			//==============================================    translate =================================== 

			for (auto& ver : translated.verticies)
			{
				ver = ver + obj.pos;
			}
			//============================================== project and draw =================================== 

			multiplyMatrixVector(translated.verticies[0], projected.verticies[0], projMatrix);
			multiplyMatrixVector(translated.verticies[1], projected.verticies[1], projMatrix);
			multiplyMatrixVector(translated.verticies[2], projected.verticies[2], projMatrix);

			normToScreen(screenWidth, screenHeight, projected);

			al_draw_triangle(
				projected.verticies[0].x,
				projected.verticies[0].y,

				projected.verticies[1].x,
				projected.verticies[1].y,

				projected.verticies[2].x,
				projected.verticies[2].y,

				al_map_rgb(255, 255, 255),
				2
			);


		}
	}
}

void ViewPort::translate(int index, float x, float y, float z)
{
	auto& g = geometry[0];
	g.pos = Vector3(g.pos.x + x, g.pos.y + y, g.pos.z + z);
}

void ViewPort::setPosition(int index, float x, float y, float z)
{
	auto& g = geometry[0];
	g.pos = Vector3(x, y, z);
}

void ViewPort::rotate(int index, float x, float y, float z)
{
	auto& g = geometry[0];
	g.rot = Vector3(g.rot.x + x, g.rot.y + y, g.rot.z + z);
}

void ViewPort::setRotation(int index, float x, float y, float z)
{
	auto& g = geometry[0];
	g.rot = Vector3(x, y, z);
}



