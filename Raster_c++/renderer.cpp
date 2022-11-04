#include<iostream>
#include"renderer.h"
#include"3dMethods.h"
#include <algorithm>


using std::string;

int screenWidth;
int screenHeight;

const float nearPlane = 0.1f;
const float farPlane = 1000.0f;
const float fov =60.0f;
float aspectRatio;


Matrix4x4 projMatrix;
 vector<Mesh>geometry;
 directionLight dirLight;
 vector<pointLight> pointLights;

 vector3 camPos;
 vector3 camRot = vector3(0.0f, 0.0f, 1.0f);

void ViewPort::InitViewPort(int ScreenWidth, int ScreenHeight)
{
	screenWidth = ScreenWidth;
	screenHeight = ScreenHeight;
	aspectRatio = (float)screenHeight / (float)screenWidth;

	projMatrix = getProjectionMat(fov, aspectRatio, farPlane, nearPlane);
}
string tempDirectoryToMesh = "house.obj";
void ViewPort::InitGeometry()
{
	geometry = vector<Mesh>{Mesh()};
	geometry[0].loadFromObj(tempDirectoryToMesh);
	geometry[0].pos = vector3(0.0f, 0.0f, 5.0f);

	dirLight = directionLight();
	dirLight.lumens = 20.0f;
	dirLight.ambientMulti = 1.0f;
	dirLight.dir = vector3(-3.0f, 1.0f, -0.5f);

	pointLights.push_back(pointLight());
	pointLights[0].color = Color(255, 255, 255);
	pointLights[0].lumens = 50.0f;
	pointLights[0].range = 15.0f;
	pointLights[0].pos = vector3(1.0f, 2.5f, 4.0f);
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

			vector3 camUp = vector3(0.0f, 1.0f, 0.0f);
			vector3 camTarget = vector3(0,0,1);
			Matrix4x4 camMatRotY = getRotMatY(camRot.y);
			Matrix4x4 camMatRotX = getRotMatZ((-camRot.x + degToRad(90.0f)) );

			

			auto lookDir =  multiplyMatrixVector(camMatRotX, camUp);
			lookDir.y = lookDir.y;
			lookDir.x = 0.0f;
			lookDir.z = 0.0f;
			lookDir = lookDir + multiplyMatrixVector(camMatRotY,camTarget);
			lookDir.x = lookDir.x / (1.0f + abs(lookDir.y));
			lookDir.z = lookDir.z / (1.0f + abs(lookDir.y));


			camTarget = camPos + lookDir;
			

			Matrix4x4 camMat = matPointAt(camPos, camTarget, camUp);
			
			Matrix4x4 viewMat = matInverse(camMat);

		for (auto tri : obj.triangles)
		{
			Triangle projected, translated,viewed;
			
			translated.verticies[0] =  multiplyMatrixVector(transform, tri.verticies[0]);
			translated.verticies[1] =  multiplyMatrixVector(transform, tri.verticies[1]);
			translated.verticies[2] =  multiplyMatrixVector(transform, tri.verticies[2]);

			projected = translated;
			//============================================= calc normals =======================================


			vector3 norm, l1, l2;
			l1 = vector3( translated.verticies[1]) - vector3 (translated.verticies[0]);
			l2 = vector3( translated.verticies[2]) - vector3( translated.verticies[0]);
			norm = crossProduct(l1, l2);
			toNormalized(norm);
			float dotProd = toDotProduct(norm,vector3( translated.verticies[0]), camPos);
			//exclude obscured
			
			if ( dotProd < 0.0f)
			{
				//============================================== Lighting ==============================================================================================
				//dir light

				vector3 direction = dirLight.dir;
				toNormalized(direction);

				float dotPod = toDotProduct(norm, direction);

				float brightness = dotPod * dirLight.lumens;
				brightness += dirLight.ambientMulti * dirLight.lumens;
				triColInfo colinfo = triColInfo();
				

				vector3 planeCenter;
				planeCenter = averagePos(projected);
				//point lights
				float brightness2 = 0.0f;
				for (auto x : pointLights)
				{
					//add check for LOS to plane
					// add check for dot product
					vector3 angle = planeCenter - x.pos;
					toNormalized(angle);
					dotProd = -toDotProduct(angle, norm);

					brightness2 += ((( x.range + 1.0f / getDistance(x.pos,planeCenter) )/ getDistance(x.pos,planeCenter))
						* x.lumens) * dotProd;
				//	std::cout << brightness2 << "\n";

				
				}	
				clamp(brightness2, 0, 255, false);

				float overallBrightness = brightness + brightness2;
				overallBrightness = clamp(overallBrightness, 0, 255, true);
				colinfo.col = Color(overallBrightness, overallBrightness, overallBrightness);
				//============================================== wolrd to view space && Clipping ===========================
				viewed.verticies[0] = multiplyMatrixVector(viewMat, projected.verticies[0]);
				viewed.verticies[1] = multiplyMatrixVector(viewMat, projected.verticies[1]);
				viewed.verticies[2] = multiplyMatrixVector(viewMat, projected.verticies[2]);

				int clippedCount = 0;
				vector<Triangle> clippedTris = vector<Triangle>{Triangle(),Triangle()};
			

				clippedCount = TriClipFromPlane(vector3(0.0f, 0.0f,nearPlane), vector3(0.0f, 0.0f, 1.0f), viewed, clippedTris[0], clippedTris[1]);

				//============================================== project  =================================== 
				for(int i = 0; i < clippedCount; i++)
				{
					projected.verticies[0] = multiplyMatrixVector(projMatrix, clippedTris[i].verticies[0]);
					projected.verticies[1] = multiplyMatrixVector(projMatrix, clippedTris[i].verticies[1]);
					projected.verticies[2] = multiplyMatrixVector(projMatrix, clippedTris[i].verticies[2]);

					projected.verticies[0] = projected.verticies[0] / projected.verticies[0].w;
					projected.verticies[1] = projected.verticies[1] / projected.verticies[1].w;
					projected.verticies[2] = projected.verticies[2] / projected.verticies[2].w;

					projected.textureCoords[0] = clippedTris[i].textureCoords[0];
					projected.textureCoords[1] = clippedTris[i].textureCoords[1];
					projected.textureCoords[2] = clippedTris[i].textureCoords[2];

					//apply lighting
					projected.colInfo = colinfo;
					toRender.push_back(projected);
				}
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
		/*
		textureTri(
			Projected.verticies[0].x, Projected.verticies[0].y,Projected.textureCoords[0].x,Projected.textureCoords[0].y,
			Projected.verticies[1].x, Projected.verticies[1].y, Projected.textureCoords[1].x, Projected.textureCoords[1].y,
			Projected.verticies[2].x, Projected.verticies[2].y, Projected.textureCoords[2].x, Projected.textureCoords[2].y,
			tempTexture
		);
		*/
		
		
		al_draw_filled_triangle(
			Projected.verticies[0].x,
			Projected.verticies[0].y,

			Projected.verticies[1].x,
			Projected.verticies[1].y,

			Projected.verticies[2].x,
			Projected.verticies[2].y,

			al_map_rgb(Projected.colInfo.col.r, Projected.colInfo.col.g, Projected.colInfo.col.b)
		);
		

		
		bool wireframe = false; // ================================================================== WIREFRAME MODE ==================================
		
		if (wireframe)
		{
			al_draw_triangle(
				Projected.verticies[0].x,
				Projected.verticies[0].y,

				Projected.verticies[1].x,
				Projected.verticies[1].y,

				Projected.verticies[2].x,
				Projected.verticies[2].y,

				al_map_rgb(255, 255, 255),
				1.0f
			);
		}
	}
}

#pragma region game renderer interaction
void ViewPort::translate(int index, float x, float y, float z)
{
	auto& g = geometry[0];
	g.pos = vector3(g.pos.x + x, g.pos.y + y, g.pos.z + z);
}

void ViewPort::setPosition(int index, float x, float y, float z)
{
	auto& g = geometry[0];
	g.pos = vector3(x, y, z);
}

void ViewPort::rotate(int index, float x, float y, float z)
{
	auto& g = geometry[0];
	auto v = vector3(g.rot.x + x, g.rot.y + y, g.rot.z + z);
	degToRad(v);
	g.rot = v;
}

void ViewPort::setRotation(int index, float x, float y, float z)
{
	auto& g = geometry[0];
	auto v = vector3(x, y, z);
		degToRad(v); 
	g.rot =v;
}

void ViewPort::camSetPos(float x, float y, float z)
{
	camPos = vector3(x, y, z);
}

void ViewPort::camSetRot(float x, float y, float z)
{
	auto v =vector3(x, y, z);
	degToRad(v);
	camRot = v;
}

#pragma endregion

