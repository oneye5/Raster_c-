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

void ViewPort::InitGeometry()
{
	geometry = vector<Mesh>{Mesh() , Mesh()};
	geometry[0].loadFromObj("mainRoom2.obj");
	geometry[0].pos = vector3(0.0f, 0.0f, 0.0f);
	geometry[0].rot = vector3(degToRad(-180.0f), 0.0f, 0.0f);

	geometry[1].loadFromObj("monke.obj");
	geometry[1].pos = vector3(0.0f, -4.0f, 20.0f);
	geometry[1].rot = vector3(0.0f, 0.0f, 1.0f);

	//init lights --

	dirLight = directionLight();
	dirLight.lumens = 70.0f;
	dirLight.ambientMulti = 0.5f;
	dirLight.dir = vector3(0.5f, -1.0f, 0.5f);
	dirLight.color = Color((unsigned char)255, (unsigned char)200, (unsigned char)200);

	pointLights.push_back(pointLight()); //monkey Light 1
	pointLights[0].color = Color((unsigned char)255, (unsigned char)0, (unsigned char)200);
	pointLights[0].lumens = 200.0f;
	pointLights[0].range = 6.0f;
	pointLights[0].pos = vector3(1.25f, -3.5f, 0.0f);

	pointLights.push_back(pointLight()); // monkey light 2
	pointLights[1].color = Color((unsigned char)0, (unsigned char)200, (unsigned char)255);
	pointLights[1].lumens = 200.0f;
	pointLights[1].range = 6.0f;
	pointLights[1].pos = vector3(-1.25f, -3.5f, 0.0f);

	pointLights.push_back(pointLight()); // room light 
	pointLights[2].color = Color((unsigned char)200, (unsigned char)200, (unsigned char)255);
	pointLights[2].lumens = 700.0f;
	pointLights[2].range = 100.0f;
	pointLights[2].pos = vector3(0.0f, -8.0f, 0.0f);

	pointLights.push_back(pointLight()); // sphere light
	pointLights[3].color = Color((unsigned char)200, (unsigned char)200, (unsigned char)255);
	pointLights[3].lumens = 1000.0f;
	pointLights[3].range = 1000.0f;
	pointLights[3].pos = vector3(-23.0f, -4.0f, 26.5f);

	pointLights.push_back(pointLight()); // player light
	pointLights[4].color = Color((unsigned char)255, (unsigned char)100, (unsigned char)255);
	pointLights[4].lumens = 500.0f;
	pointLights[4].range = 100.0f;
	pointLights[4].pos = vector3(-0.0f, -0.0f,0.0f);

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
				vector<Color> lightCols;

				vector3 direction = dirLight.dir;
				toNormalized(direction);

				float dotPod =  toDotProduct(direction, norm);

				float brightness = dotPod * dirLight.lumens;
				brightness += dirLight.ambientMulti * dirLight.lumens;
				triColInfo colinfo = triColInfo();
				
				float r = clamp((float)dirLight.color.r / 255.0f, 0, 1.0f, false);
				float g = clamp((float)dirLight.color.g / 255.0f, 0, 1.0f, false);
				float b = clamp((float)dirLight.color.b / 255.0f, 0, 1.0f, false);

				lightCols.push_back(Color(clamp(r * brightness, 0, 255, true), clamp(g * brightness, 0, 255, true), clamp(b * brightness, 0, 255, true)));

				vector3 planeCenter;
				planeCenter = averagePos(projected);

				//point lights
				for (auto& x : pointLights)
				{
					float brightness2 = 0.0f;
					//add check for LOS to plane
					// add check for dot product
					float dist = getDistance(x.pos, planeCenter);

					if (getDistance(x.pos, planeCenter) > x.range)
						continue;

					vector3 angle = planeCenter - x.pos;
					toNormalized(angle);
					dotPod = abs( toDotProduct(angle, norm));

					brightness2 = dotPod * x.lumens;
					brightness2 *= (x.range / (dist + 1)) / x.range;


					float r = clamp( (float)x.color.r / 255.0f,0,1.0f,false);
					float g = clamp((float)x.color.g / 255.0f, 0, 1.0f, false);
					float b = clamp((float)x.color.b / 255.0f, 0, 1.0f, false);

					lightCols.push_back(Color(clamp(r * brightness2, 0, 255, true), clamp(g * brightness2, 0, 255, true), clamp(b * brightness2, 0, 255, true)));
				}	
				
				//add all together
				
				float rOut = 0.0f;
				float gOut = 0.0f;
				float bOut = 0.0f;
				for (auto x : lightCols)
				{
					rOut += x.r;
					gOut += x.g;
					bOut += x.b;
				}
				rOut = clamp(rOut, 0, 255, true);
				gOut = clamp(gOut, 0, 255, true);
				bOut = clamp(bOut, 0, 255, true);


				colinfo.col = Color(rOut, gOut,bOut);
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

				//	projected.textureCoords[0] = clippedTris[i].textureCoords[0];
			//		projected.textureCoords[1] = clippedTris[i].textureCoords[1];
			//		projected.textureCoords[2] = clippedTris[i].textureCoords[2];

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
	auto& g = geometry[index];
	auto v = vector3(g.rot.x + x, g.rot.y + y, g.rot.z + z);
	degToRad(v);
	g.rot = v;
}

void ViewPort::setRotation(int index, float x, float y, float z)
{
	auto& g = geometry[index];
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

void ViewPort::setLightPos(int index,float x, float y, float z)
{
	auto& pl = pointLights[index];

	pl.pos = vector3(x, y, z);
}

#pragma endregion

