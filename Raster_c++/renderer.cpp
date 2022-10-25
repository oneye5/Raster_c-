#include<iostream>
#include"renderer.h"
#include"3dMethods.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro.h"
int screenWidth;
int screenHeight;

const float nearPlane = 0.0f;
const float farPlane = 1000.0f;
const float fov = 0.025f;
float aspectRatio;
float fovRad = degToRad(1.0f / tanf(fov * 0.5));

Matrix4x4 projMatrix;

Mesh tempMesh = cubeMesh();

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
	
	tempMesh = translate(Vector3(4.5f, -1.0f,5.0f), cubeMesh());
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


			std::cout<<"\nxy pos : " << projected.verticies[0].x << " " << projected.verticies[0].y;
		}
	}
}
void ViewPort::render()
{
	
	drawGeometry(vector<Mesh>{ tempMesh});
}



