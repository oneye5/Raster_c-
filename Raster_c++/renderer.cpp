#include<iostream>
#include"renderer.h"
#include"3dMethods.h"
#include <algorithm>
#include<future>


using std::string;

int screenWidth;
int screenHeight;

const float nearPlane = 0.1f;
const float farPlane = 1.0f;
const float fov = 100.0f;
float aspectRatio;


Matrix4x4 projMatrix;
 directionLight dirLight;
 vector<pointLight> pointLights;

 vector3 camPos;
 vector3 camRot = vector3(0.0f, 0.0f, 1.0f);

 sceneDesc sceneMain;

void ViewPort::InitViewPort(int ScreenWidth, int ScreenHeight)
{
	screenWidth = ScreenWidth;
	screenHeight = ScreenHeight;
	aspectRatio = (float)screenHeight / (float)screenWidth;

	projMatrix = getProjectionMat(fov, aspectRatio, farPlane, nearPlane);
}

void ViewPort::InitGeometry()
{
	

	//init lights --

	dirLight = directionLight();
	dirLight.lumens = 70.0f;
	dirLight.ambientMulti = 0.5f;
	dirLight.dir = vector3(0.5f, -1.0f, 0.5f);
	dirLight.color = Color((unsigned char)255, (unsigned char)100, (unsigned char)200);

	pointLights.push_back(pointLight()); //sphereL
	pointLights[0].color = Color((unsigned char)200, (unsigned char)200, (unsigned char)255);
	pointLights[0].lumens = 400.0f;
	pointLights[0].range = 10.0f;
	pointLights[0].pos = vector3(-3.29036f, -1.16f, -11.601f);

	pointLights.push_back(pointLight());  // hallL
	pointLights[1].color = Color((unsigned char)200, (unsigned char)255, (unsigned char)255);
	pointLights[1].lumens = 100.0f;
	pointLights[1].range = 4.0f;
	pointLights[1].pos = vector3(1.64197f, -1.0f, 0.297399f);

	pointLights.push_back(pointLight()); // tree
	pointLights[2].color = Color((unsigned char)255, (unsigned char)0, (unsigned char)200);
	pointLights[2].lumens = 400.0f;
	pointLights[2].range = 20.0f;
	pointLights[2].pos = vector3(35.4461f, -1.955f, 14.3487f);

	pointLights.push_back(pointLight()); // tree2
	pointLights[3].color = Color((unsigned char)255, (unsigned char)0, (unsigned char)200);
	pointLights[3].lumens = 400.0f;
	pointLights[3].range = 20.0f;
	pointLights[3].pos = vector3(26.5184f, -7.04521f, 27.903f);

	pointLights.push_back(pointLight()); //highlight
	pointLights[4].color = Color((unsigned char)255, (unsigned char)255, (unsigned char)200);
	pointLights[4].lumens = 500.0f;
	pointLights[4].range = 100.0f;
	pointLights[4].pos = vector3(16.5254f, -9.56f, 18.3688f);

	pointLights.push_back(pointLight()); //CITY LIGHT
	pointLights[5].color = Color((unsigned char)255, (unsigned char)0, (unsigned char)200);
	pointLights[5].lumens = 500.0f;
	pointLights[5].range = 100.0f;
	pointLights[5].pos = vector3(73.5361f, -4.59f, 37.8652f);
	
	pointLights.push_back(pointLight()); //CITY LIGHT 2
	pointLights[6].color = Color((unsigned char)100, (unsigned char)240, (unsigned char)255);
	pointLights[6].lumens = 500.0f;
	pointLights[6].range = 100.0f;
	pointLights[6].pos = vector3(80.5604f, -5.55999f, 8.2133f);
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
	8  = 17.19 good sweetspot
	10 = 16.39
	12 = 16.88
	16 = 16.40
	24 = 16.22
	32 = 17.45
	64 = 17.58
	128= 16.75

*/

void calculateTriBatch(vector<Triangle>* tris, vector<Triangle>* toRender, Matrix4x4 transform, Matrix4x4 viewMat, int from,int to)
{
	for (int i = from; i < to;i++)
	{
		auto& tri = tris->at(i);

		Triangle triProjected;

		triProjected.verticies[1] = multiplyMatrixVector(transform, tri.verticies[1]);
		triProjected.verticies[2] = multiplyMatrixVector(transform, tri.verticies[2]);
		triProjected.verticies[0] = multiplyMatrixVector(transform, tri.verticies[0]);
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

			//============================================== Lighting ==============================================================================================
#pragma region lighting
			//direction lighting
			triColInfo colinfo = triColInfo();
			vector<Color>  colors;
			vector3 direction = dirLight.dir;
			toNormalized(direction);
			float dotPod = toDotProduct(direction, norm);
			float brightness = dotPod * dirLight.lumens;
			brightness += dirLight.ambientMulti * dirLight.lumens;

			float r = dirLight.color.r; r = r / 255.0f; r *= brightness; r = clamp(r, 0, 255, true);
			float g = dirLight.color.g; g = g / 255.0f; g *= brightness; g = clamp(g, 0, 255, true);
			float b = dirLight.color.b; b = b / 255.0f; b *= brightness; b = clamp(b, 0, 255, true);
			
			colors.push_back(Color(r, g, b)); colors.push_back(Color(r, g, b)); colors.push_back(Color(r, g, b));

			for (int i = 0; i < 3; i++) //per vert lighting
			{
				Color vertCol;
				vector3 vertPos = tri.verticies[i];
				for (auto& x : pointLights)
				{
					float dist = getDistance(x.pos, vertPos);

					if (dist > x.range)
						continue;

					vector3 angle = vertPos - x.pos;
					toNormalized(angle);
					dotPod = abs(toDotProduct(angle, norm));
					brightness = dotPod * x.lumens;
					brightness *= (x.range / (dist + 1)) / x.range;

					r = x.color.r; r = r / 255.0f;r *= brightness;	r = clamp(r, 0, 255, true);
					g = x.color.g; g = g / 255.0f; g *= brightness; g = clamp(g, 0, 255, true);
					b = x.color.b; b = b / 255.0f; b *= brightness; b = clamp(b, 0, 255, true);

					r = r + colors[i].r;
					g = g + colors[i].g;
					b = b + colors[i].b;

					colors[i] = Color(
						clamp(r, 0, 255, true),
						clamp(g, 0, 255, true),
						clamp(b, 0, 255, true)
					);
				}
			}
			colinfo.al_col = vector<ALLEGRO_COLOR>();
			colinfo.al_col.push_back(al_map_rgb(colors[0].r, colors[0].g, colors[0].b));
			colinfo.al_col.push_back(al_map_rgb(colors[1].r, colors[1].g, colors[1].b));
			colinfo.al_col.push_back(al_map_rgb(colors[2].r, colors[2].g, colors[2].b));
				//dir light
			/*
			
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


			colinfo.al_col = al_map_rgb(rOut, gOut, bOut);
			*/
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
				triProjected.zAve = averagePos(triProjected).z;

				std::lock_guard<std::mutex> lock(mutex);
				toRender->push_back(triProjected);
			}
			
		}
	}
}
void ViewPort::render()
{
	vector<Triangle> toRender;
	for (auto& obj : sceneMain.geometry)
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

			bool init = false;
			int currentStart = 0;
			for (int i = 0; i < calcBatchCount; i++)
			{
				int currentEnd = roundf((float)obj.triangles.size() / (float)calcBatchCount);
				currentEnd += currentStart + 1;
				currentEnd = clamp(currentEnd, 0.0f, obj.triangles.size(), true);

				futures.push_back(
					std::async(std::launch::async, calculateTriBatch, &obj.triangles, &toRender, transform, viewMat, currentStart, currentEnd)
				);


				currentStart = currentEnd;
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

			float z1 = t1.zAve;
			float z2 = t2.zAve;
			return z1 > z2;
		});
	//==============================================	draw	 =================================== 
	for (auto& t : toRender)
	{
		ALLEGRO_VERTEX vtx[3];
		 vtx[0].x = t.verticies[0].x;
		 vtx[0].y = t.verticies[0].y;
		 vtx[0].z = 0;
		 vtx[0].color = t.colInfo.al_col[0];
		 vtx[0].u = 1;
		 vtx[0].v = 1;

		  vtx[1].x = t.verticies[1].x;
		 vtx[1].y = t.verticies[1].y;
		 vtx[1].z = -0;
		 vtx[1].color = t.colInfo.al_col[1];
		 vtx[1].u = 100;
		 vtx[1].v = 100;

		 vtx[2].x = t.verticies[2].x;
		 vtx[2].y = t.verticies[2].y;
		 vtx[2].z = -0;
		 vtx[2].color = t.colInfo.al_col[2];
		 vtx[2].u = 100;
		 vtx[2].v = 100;

		 

		al_draw_prim(vtx, NULL,NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_FAN);
		/*
		al_draw_filled_triangle(
		t.verticies[0].x,
		t.verticies[0].y,

		t.verticies[1].x,
		t.verticies[1].y,

		t.verticies[2].x,
		t.verticies[2].y,

		t.colInfo.al_col
		);
		*/


		//al_draw_triangle(	t.verticies[0].x,t.verticies[0].y,t.verticies[1].x,	t.verticies[1].y,t.verticies[2].x,t.verticies[2].y,t.colInfo.al_col,1.0f);
	}
	
	al_flip_display();
}

#pragma region game renderer interaction
void ViewPort::translate(int index, float x, float y, float z)
{
	auto& g = sceneMain.geometry[0];
	g.pos = vector3(g.pos.x + x, g.pos.y + y, g.pos.z + z);
}

void ViewPort::setPosition(int index, float x, float y, float z)
{
	auto& g = sceneMain.geometry[0];
	g.pos = vector3(x, y, z);
}

void ViewPort::rotate(int index, float x, float y, float z)
{
	auto& g = sceneMain.geometry[index];
	auto v = vector3(g.rot.x + x, g.rot.y + y, g.rot.z + z);
	degToRad(v);
	g.rot = v;
}

void ViewPort::setRotation(int index, float x, float y, float z)
{
	auto& g = sceneMain.geometry[index];
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

