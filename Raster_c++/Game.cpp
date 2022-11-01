#include "Game.h"

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



	vector3 zchange,xchange,ychange;
	vector3 rawInput = vector3(input.x, input.y, input.z);

	zchange = camForward(vector3(0.0f, gameCamRot.y, 0.0f)) * moveMulti;
	zchange = zchange * input.z;

	ychange = input.y * moveMulti;

	zchange = camRight(vector3(0.0f, gameCamRot.y, 0.0f)) * moveMulti;
	zchange = xchange * input.x;

	vector3 change = zchange + xchange + ychange;
	gameCamPos = gameCamPos + change;

	
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
