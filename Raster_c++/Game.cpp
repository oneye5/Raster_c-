#include "Game.h"
#include "3dGameMethods.h"
#include"renderer.h"
#include<windows.system.threading.h>

ViewPort vp = ViewPort();
vector3 gameCamPos = vector3(0.0f,0.0f,0.0f);
vector3 gameCamRot = vector3(0.0f,0.0f,1.0f);
Input input = Input();
vector3 moveMulti = vector3(0.1f, 0.1f, 0.1f);
float fps=0.0f;

int renderPass()
{
	vp.camSetPos(gameCamPos.x, gameCamPos.y, gameCamPos.z);
	vp.camSetRot(gameCamRot.x, gameCamRot.y, gameCamRot.z);

	al_clear_to_color(al_map_rgb(0, 0,50));
	vp.render();
	int returnCode = windowManager.windowTick();
	
	
	return returnCode;

}
void playerControls()
{
	input.tick(windowManager.getDisplay());	
	gameCamRot = gameCamRot + vector3(input.my, -input.mx, 0.0f);
	clipRot(gameCamRot);

	vector3 forward = camForward(gameCamRot);
	vector3 right = camRight(gameCamRot);

	
	vector3 x = right * input.x ;
	vector3 y = vector3(0.0f, input.y, 0.0f);
	vector3 z = forward * input.z;

	gameCamPos = gameCamPos +z - x - y;
}

void Game::Start()
{
	windowManager.initWindow();
	vp.InitViewPort(windowManager.getSetWindow()[0], windowManager.getSetWindow()[1]);
	vp.InitGeometry();
}

int Game::Update(float deltaTime) 
{
	fps = (1.0f / deltaTime) * 1000.0f;
	playerControls();
	
	
	vp.setRotation(0, 180.0f, 0.0f, 0.0f);


	
	//std::cout << fps << "\n";
	return renderPass(); //returns -1 on quit
}
