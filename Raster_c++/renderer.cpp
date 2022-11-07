#include<iostream>
#include"renderer.h"
#include"3dMethods.h"
#include <algorithm>
#include<future>

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

std::vector<std::future<void>> futures;
static std::mutex mutex;
const int calcBatchCount = 8;

/*
	AVE FPS PER BATCH NUM
	1  = 15.78
	2  = 16.32
	4  = 16.50
	6  = 16.77
	8  = 17.19
	10 = 16.39
	12 = 16.88
	16 = 16.40
	24 = 16.22
	32 = 17.45
	64 = 17.58
	128= 16.75

*/

void calculateTriBatch(vector<Triangle> tris, vector<Triangle>* toRender, Matrix4x4 transform, Matrix4x4 viewMat)
{
	for (auto& tri : tris)
	{
		Triangle triProjected;

		triProjected.verticies[0] = multiplyMatrixVector(transform, tri.verticies[0]);
		triProjected.verticies[1] = multiplyMatrixVector(transform, tri.verticies[1]);
		triProjected.verticies[2] = multiplyMatrixVector(transform, tri.verticies[2]);
		//============================================= calc normals =======================================


		vector3 norm, l1, l2;
		l1 = vector3(triProjected.verticies[1]) - vector3(triProjected.verticies[0]);
		l2 = vector3(triProjected.verticies[2]) - vector3(triProjected.verticies[0]);
		norm = crossProduct(l1, l2);
		toNormalized(norm);
		float dotProd = toDotProduct(norm, vector3(triProjected.verticies[0]), camPos);
		//exclude obscured

		if (dotProd < 0.0f)
		{
#
			//============================================== Lighting ==============================================================================================
#pragma region lighting
				//dir light
			vector<Color> lightCols;

			vector3 direction = dirLight.dir;
			toNormalized(direction);

			float dotPod = toDotProduct(direction, norm);

			float brightness = dotPod * dirLight.lumens;
			brightness += dirLight.ambientMulti * dirLight.lumens;
			triColInfo colinfo = triColInfo();

			float r = clamp((float)dirLight.color.r / 255.0f, 0, 1.0f, false);
			float g = clamp((float)dirLight.color.g / 255.0f, 0, 1.0f, false);
			float b = clamp((float)dirLight.color.b / 255.0f, 0, 1.0f, false);

			lightCols.push_back(Color(clamp(r * brightness, 0, 255, true), clamp(g * brightness, 0, 255, true), clamp(b * brightness, 0, 255, true)));

			vector3 planeCenter;
			planeCenter = averagePos(triProjected);

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
				dotPod = abs(toDotProduct(angle, norm));

				brightness2 = dotPod * x.lumens;
				brightness2 *= (x.range / (dist + 1)) / x.range;


				float r = clamp((float)x.color.r / 255.0f, 0, 1.0f, false);
				float g = clamp((float)x.color.g / 255.0f, 0, 1.0f, false);
				float b = clamp((float)x.color.b / 255.0f, 0, 1.0f, false);

				lightCols.push_back(Color(clamp(r * brightness2, 0, 255, true), clamp(g * brightness2, 0, 255, true), clamp(b * brightness2, 0, 255, true)));
			}

			//add all together

			float rOut = 0.0f;
			float gOut = 0.0f;
			float bOut = 0.0f;
			for (auto& x : lightCols)
			{
				rOut += x.r;
				gOut += x.g;
				bOut += x.b;
			}
			rOut = clamp(rOut, 0, 255, true);
			gOut = clamp(gOut, 0, 255, true);
			bOut = clamp(bOut, 0, 255, true);


			colinfo.col = Color(rOut, gOut, bOut);
#pragma endregion
			//============================================== wolrd to view space && Clipping ===========================
			triProjected.verticies[0] = multiplyMatrixVector(viewMat, triProjected.verticies[0]);
			triProjected.verticies[1] = multiplyMatrixVector(viewMat, triProjected.verticies[1]);
			triProjected.verticies[2] = multiplyMatrixVector(viewMat, triProjected.verticies[2]);

			int clippedCount = 0;
			vector<Triangle> clippedTris = vector<Triangle>{ Triangle(),Triangle() };

			clippedCount = TriClipFromPlane(vector3(0.0f, 0.0f, nearPlane), vector3(0.0f, 0.0f, 1.0f), triProjected, clippedTris[0], clippedTris[1]);

			for (int i = 0; i < clippedCount; i++)
			{
				triProjected.verticies[0] = multiplyMatrixVector(projMatrix, clippedTris[i].verticies[0]);
				triProjected.verticies[1] = multiplyMatrixVector(projMatrix, clippedTris[i].verticies[1]);
				triProjected.verticies[2] = multiplyMatrixVector(projMatrix, clippedTris[i].verticies[2]);

				triProjected.verticies[0] = triProjected.verticies[0] / triProjected.verticies[0].w;
				triProjected.verticies[1] = triProjected.verticies[1] / triProjected.verticies[1].w;
				triProjected.verticies[2] = triProjected.verticies[2] / triProjected.verticies[2].w;

				triProjected.colInfo = colinfo;

				normToScreen(screenWidth, screenHeight, triProjected);

				std::lock_guard<std::mutex> lock(mutex);
				toRender->push_back(triProjected);
			}
			
		}
	}
}
const int drawBatchCount = 8;
void drawBatch(vector<Triangle>)
{

}
void ViewPort::render()
{
	vector<Triangle> toRender;
	for (auto& obj : geometry)
	{
		//================================================= rotate & translate =====================================
#pragma region setupVars
		Matrix4x4 matrixRotZ, matrixRotX;
		matrixRotZ = getRotMatZ(obj.rot.z);
		matrixRotX = getRotMatX(obj.rot.x);

			Matrix4x4 transform = matrixRotZ * matrixRotX;
			transform = transform * getTranslationMat(obj.pos);

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
#pragma endregion
			vector<vector<Triangle>> batches;
			int batchIndex = 0;
			for (int i = 0; i < calcBatchCount +1; i++)//populate batches
			{
				batches.push_back(vector<Triangle>());
			}

			for (auto& tri : obj.triangles)
			{
				if (batchIndex > calcBatchCount)
				batchIndex = 0;

				batches[batchIndex].push_back(tri);
			
				batchIndex++;
			}

		for (auto& x : batches)
		{
				futures.push_back(
					std::async(std::launch::async, calculateTriBatch, x, &toRender, transform, viewMat)
				);
		}
		
	}
	for (int i = 0; i < futures.size(); i++) //wait for threads
	{
		futures[i].wait();
	}
		futures.clear();
	//============================================== sort by depth  =================================== 
	std::sort(toRender.begin(), toRender.end(), [](Triangle &t1, Triangle &t2)
		{

			float z1 = averagePos(t1).z;
			float z2 =averagePos(t2).z;
			return z1 > z2;
		});
	//==============================================	draw	 =================================== 
	for (auto& finnishedTri : toRender)
	{
		
		/*
		textureTri(
			Projected.verticies[0].x, Projected.verticies[0].y,Projected.textureCoords[0].x,Projected.textureCoords[0].y,
			Projected.verticies[1].x, Projected.verticies[1].y, Projected.textureCoords[1].x, Projected.textureCoords[1].y,
			Projected.verticies[2].x, Projected.verticies[2].y, Projected.textureCoords[2].x, Projected.textureCoords[2].y,
			tempTexture
		);
		*/
		
		
		al_draw_filled_triangle(
			finnishedTri.verticies[0].x,
			finnishedTri.verticies[0].y,

			finnishedTri.verticies[1].x,
			finnishedTri.verticies[1].y,

			finnishedTri.verticies[2].x,
			finnishedTri.verticies[2].y,

			al_map_rgb(finnishedTri.colInfo.col.r, finnishedTri.colInfo.col.g, finnishedTri.colInfo.col.b)
		);
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

