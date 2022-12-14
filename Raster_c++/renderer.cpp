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
	sceneMain = sceneDesc();
	Mesh mesh = Mesh();
//	mesh.loadFromObj("test.obj");
	vector3 rot = vector3(degToRad(180), degToRad(0), degToRad(0));

	mesh.loadFromFile("world1.obj");
	sceneMain.geometry.push_back(mesh);
	sceneMain.geometry[0].init(0);
	sceneMain.geometry[0].rot = rot;

	mesh = Mesh();
	mesh.loadFromFile("world2.obj");
	sceneMain.geometry.push_back(mesh);
	sceneMain.geometry[1].init(1);
	sceneMain.geometry[1].rot = rot;

	mesh = Mesh();
	mesh.loadFromFile("world3.obj");
	sceneMain.geometry.push_back(mesh);
	sceneMain.geometry[2].init(2);
	sceneMain.geometry[2].rot = rot;

	mesh = Mesh();
	mesh.loadFromFile("world4.obj");
	sceneMain.geometry.push_back(mesh);
	sceneMain.geometry[3].init(3);
	sceneMain.geometry[3].rot = rot;

	mesh = Mesh();
	mesh.loadFromFile("world5.obj");
	sceneMain.geometry.push_back(mesh);
	sceneMain.geometry[4].init(4);
	sceneMain.geometry[4].rot = rot;

	mesh = Mesh();
	mesh.loadFromFile("world6.obj");
	sceneMain.geometry.push_back(mesh);
	sceneMain.geometry[5].init(5);
	sceneMain.geometry[5].rot = rot;
//	std::cout <<"parent5mesh " << sceneMain.geometry[0].triangles[0].parentMesh<<" ";
	//init lights --

 auto	dirLight = directionLight();
	dirLight.lumens = 10.0f;
	dirLight.ambientMulti = 0.5f;
	dirLight.dir = vector3(0.5f, -1.0f, 0.5f);
	dirLight.color = Color((unsigned char)255, (unsigned char)100, (unsigned char)200);

	sceneMain.dirLight = dirLight;

	auto pLight = pointLight();
	pLight.color = Color((unsigned char)200, (unsigned char)255, (unsigned char)255);
	pLight.lumens = 2000.0f;
	pLight.pos = vector3(0.356367f, -9.39999f, 3.78953f);
	pLight.range = 1000.0f;
	sceneMain.pLights.push_back(pLight);
	

	pLight = pointLight();
	pLight.color = Color((unsigned char)255, (unsigned char)100, (unsigned char)255);
	pLight.lumens = 2000.0f;
	pLight.pos = vector3(-55.9074f, -14.4f, 8.11035f);
	pLight.range = 1000.0f;
	sceneMain.pLights.push_back(pLight);

	
	pLight = pointLight();
	pLight.color = Color((unsigned char)255, (unsigned char)255, (unsigned char)255);
	pLight.lumens = 2000.0f;
	pLight.pos = vector3(-50.0582f, -94.7f, 56.3013f);
	pLight.range = 50000.0f;
	sceneMain.pLights.push_back(pLight);

	pLight = pointLight();
	pLight.color = Color((unsigned char)255, (unsigned char)255, (unsigned char)255);
	pLight.lumens = 20000.0f;
	pLight.pos = vector3(-51.6908f, -129.65f, 135.17f);
	pLight.range = 50000.0f;
	sceneMain.pLights.push_back(pLight);
}

std::vector<std::future<void>> futures;
static std::mutex mutex;
const int BatchCount = 8;

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

void initMats(Matrix4x4& viewMat, Matrix4x4& transform, Mesh& obj)
{
	Matrix4x4 matrixRotZ, matrixRotX;
	matrixRotZ = getRotMatZ(obj.rot.z);
	matrixRotX = getRotMatX(obj.rot.x);

	transform = matrixRotZ * matrixRotX;
	transform = transform * getTranslationMat(obj.pos);

	vector3 camUp = vector3(0.0f, 1.0f, 0.0f);
	vector3 camTarget = vector3(0, 0, 1);
	Matrix4x4 camMatRotY = getRotMatY(camRot.y);
	Matrix4x4 camMatRotX = getRotMatZ((-camRot.x + degToRad(90.0f)));



	auto lookDir = multiplyMatrixVector(camMatRotX, camUp);
	lookDir.y = lookDir.y;
	lookDir.x = 0.0f;
	lookDir.z = 0.0f;
	lookDir = lookDir + multiplyMatrixVector(camMatRotY, camTarget);
	lookDir.x = lookDir.x / (1.0f + abs(lookDir.y));
	lookDir.z = lookDir.z / (1.0f + abs(lookDir.y));


	camTarget = camPos + lookDir;


	viewMat = matPointAt(camPos, camTarget, camUp);
	viewMat = matInverse(viewMat);
#pragma endregion
}

vector<Triangle> toRender;

void calculateBatch(vector<Triangle> tris, int from,int to,vector<Triangle>* toRender)
{
	Matrix4x4 viewMat, transform;
	int currentMesh = -1;


	//std::cout <<"\n" << tris.size() << " end size from to = " << from << " " << to << "\n";

	for (int i = from; i < to; i++)
	{
		auto& tri = tris[i];

		if (tri.parentMesh != currentMesh) //if incorect matrix generate new
		{
			currentMesh = tri.parentMesh;
			initMats(viewMat, transform, sceneMain.geometry[0]);

		}

				//start


#pragma region projection
		Triangle triProjected = Triangle();

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

		if (dotProd <= 0.0f)
		{

			//============================================== Lighting ==============================================================================================
#pragma region lighting
			//direction lighting
			triColInfo colinfo = triColInfo();
			vector<Color>  colors;

			vector3 direction = sceneMain.dirLight.dir;
			toNormalized(direction);
			float dotPod = toDotProduct(direction, norm);
			float brightness = dotPod * sceneMain.dirLight.lumens;
			brightness += sceneMain.dirLight.ambientMulti * sceneMain.dirLight.lumens;

			float r = sceneMain.dirLight.color.r; r = r / 255.0f; r *= brightness; r = clamp(r, 0, 255, true);
			float g = sceneMain.dirLight.color.g; g = g / 255.0f; g *= brightness; g = clamp(g, 0, 255, true);
			float b = sceneMain.dirLight.color.b; b = b / 255.0f; b *= brightness; b = clamp(b, 0, 255, true);

			colors.push_back(Color(r, g, b)); colors.push_back(Color(r, g, b)); colors.push_back(Color(r, g, b));

			for (int i = 0; i < 3; i++) //per vert lighting
			{
				Color vertCol;
				vector3 vertPos = tri.verticies[i];
				for (auto& x : sceneMain.pLights)
				{
					float dist = getDistance(x.pos, vertPos);

					if (dist > x.range)
						continue;

					vector3 angle = vertPos - x.pos;
					toNormalized(angle);
					dotPod = abs(toDotProduct(angle, norm));
					brightness = dotPod * x.lumens;
					brightness *= (x.range / (dist + 1)) / x.range;

					r = x.color.r; r = r / 255.0f; r *= brightness;	r = clamp(r, 0, 255, true);
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

				normToScreen(screenWidth, screenHeight, triProjected);
				triProjected.zAve = averagePos(triProjected).z;
				triProjected.parentMesh = tri.parentMesh;
				triProjected.colInfo = colinfo;
				triProjected.uvs = tri.uvs;

				if(sceneMain.geometry[triProjected.parentMesh].texture.tex != nullptr)
				unNormalizeUvs(triProjected, sceneMain.geometry[triProjected.parentMesh].texture);
#pragma endregion
				//end
				std::lock_guard<std::mutex> lock(mutex);
				toRender->push_back(triProjected);
			}
		}
	}
}
void batchAlloc()
{
	vector<Triangle> allTris;
	for (auto m : sceneMain.geometry)
	{
		allTris.insert(allTris.end(), m.triangles.begin(), m.triangles.end());
	}
	//tris per batch
	int trisPerBatch = roundf( (float)allTris.size() / (float)BatchCount) + 1;
	int from = 0;
	int to = 0;

	for (int batch = 0; batch < BatchCount; batch++)
	{
		to = (batch + 1) * trisPerBatch;
		if (to > allTris.size())
			to = allTris.size();


		futures.push_back(
			std::async(std::launch::async, calculateBatch,
				allTris,from,to,&toRender)
		);
		from = to;
	}
}
void ViewPort::render()
{
	batchAlloc();

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
		 vtx[0].u = t.uvs[0].x;
		 vtx[0].v = t.uvs[0].y;

		  vtx[1].x = t.verticies[1].x;
		 vtx[1].y = t.verticies[1].y;
		 vtx[1].z = -0;
		 vtx[1].color = t.colInfo.al_col[1];
		 vtx[1].u = t.uvs[1].x;
		 vtx[1].v = t.uvs[1].y;

		 vtx[2].x = t.verticies[2].x;
		 vtx[2].y = t.verticies[2].y;
		 vtx[2].z = -0;
		 vtx[2].color = t.colInfo.al_col[2];
		 vtx[2].u =t.uvs[2].x;
		 vtx[2].v =t.uvs[2].y;

		 
	//	 sceneMain.geometry[t.parentMesh].texture.tex;
		 if(sceneMain.geometry[t.parentMesh].texture.tex == nullptr)
			 al_draw_prim(vtx, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_FAN);
		 else
		al_draw_prim(vtx, NULL, sceneMain.geometry[t.parentMesh].texture.tex, 0, 3, ALLEGRO_PRIM_TRIANGLE_FAN);
	}
	
	al_flip_display();
	toRender.clear();
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
	//auto& pl = pointLights[index];

	//pl.pos = vector3(x, y, z);
}

#pragma endregion

