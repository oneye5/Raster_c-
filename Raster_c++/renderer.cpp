#include<iostream>
#include"renderer.h"
#include"3dMethods.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro.h"
#include <algorithm>
using std::string;

int screenWidth;
int screenHeight;

const float nearPlane = 0.1f;
const float farPlane = 1000.0f;
const float fov = 90.0f;
float aspectRatio;


Matrix4x4 projMatrix;
 vector<Mesh>geometry;
 directionLight dirLight;
 Vector3 camPos;


void ViewPort::InitViewPort(int ScreenWidth, int ScreenHeight)
{
	screenWidth = ScreenWidth;
	screenHeight = ScreenHeight;
	aspectRatio = (float)screenHeight / (float)screenWidth;

	projMatrix = getProjectionMat(fov, aspectRatio, farPlane, nearPlane);
}
string tempDirectoryToMesh = "monke.obj";
void ViewPort::InitGeometry()
{
	geometry = vector<Mesh>{Mesh()};
	geometry[0].loadFromObj(tempDirectoryToMesh);
	geometry[0].pos = Vector3(0.0f, 0.0f, 3.0f);

	dirLight = directionLight();
	dirLight.lumens = 90;
	dirLight.dir = Vector3(0.0f, 0.0f, -0.5f);
}

void ViewPort::render()
{
	vector<Triangle> toRender;
	for (auto obj : geometry)
	{
		//================================================= rotate & translate =====================================
		Matrix4x4 matrixRotZ, matrixRotX;
		matrixRotZ = getRotMatZ(obj.rot.z);
		matrixRotX = getRotMatX(obj.rot.x);

		Matrix4x4 offset = getTranslationMat(obj.pos);
			Matrix4x4 transform = getEmptyMat();
			transform = matrixRotZ * matrixRotX;
			transform = transform * offset;

		for (auto tri : obj.triangles)
		{
			Triangle projected, translated;
			
			translated.verticies[0] =  multiplyMatrixVector(transform, tri.verticies[0]);
			translated.verticies[1] =  multiplyMatrixVector(transform, tri.verticies[1]);
			translated.verticies[2] =  multiplyMatrixVector(transform, tri.verticies[2]);

			projected = translated;
			//============================================= calc normals =======================================


			Vector3 norm, l1, l2;
			l1 = Vector3( translated.verticies[1]) - Vector3 (translated.verticies[0]);
			l2 = Vector3( translated.verticies[2]) - Vector3( translated.verticies[0]);
			norm = crossProduct(l1, l2);
			toNormalized(norm);
			float dotProd = toDotProduct(norm,Vector3( translated.verticies[0]), camPos);
			//exclude obscured
			
			if ( dotProd < 0.0f)
			{
				//============================================== Lighting ===========================================
				Vector3 direction = dirLight.dir;
				toNormalized(direction);

				float dotPod = toDotProduct(norm, direction);

				float brightness = dotPod * dirLight.lumens;
				brightness = clamp(brightness, 0, 255, true);
				projected.col = Color(brightness, brightness , brightness);
				//============================================== project  =================================== 

				projected.verticies[0]=  multiplyMatrixVector(projMatrix,projected.verticies[0]);
				projected.verticies[1] = multiplyMatrixVector(projMatrix, projected.verticies[1]);
				projected.verticies[2] = multiplyMatrixVector(projMatrix, projected.verticies[2]);

				projected.verticies[0] = projected.verticies[0] / projected.verticies[0].w;
				projected.verticies[1] = projected.verticies[1] / projected.verticies[1].w;
				projected.verticies[2] = projected.verticies[2] / projected.verticies[2].w;

				toRender.push_back(projected);
			}
		}
	}
	//============================================== sort by depth  =================================== 
	std::sort(toRender.begin(), toRender.end(), [](Triangle &t1, Triangle &t2)
		{

			float z1 = averagePos(t1).z;
			float z2 =averagePos(t2).z;
			return z1 > z2;
		});
	//==============================================	draw	 =================================== 
	for (auto& Projected : toRender)
	{
		normToScreen(screenWidth, screenHeight, Projected);
		al_draw_filled_triangle(
			Projected.verticies[0].x,
			Projected.verticies[0].y,

			Projected.verticies[1].x,
			Projected.verticies[1].y,

			Projected.verticies[2].x,
			Projected.verticies[2].y,

			al_map_rgb(Projected.col.r, Projected.col.g, Projected.col.b)
		);
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



