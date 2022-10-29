#include "Game.h"
#include "windowManager.h"
#include"renderer.h"
#include<windows.system.threading.h>

ViewPort vp = ViewPort();
Vector3 gameCamPos = Vector3(0.0f,0.0f,0.0f);
Vector3 gameCamRot = Vector3(0.0f,0.0f,1.0f);
Input input = Input();
Vector3 moveMulti = Vector3(0.1f, 0.1f, 0.1f);

int renderPass()
{
	vp.camSetPos(gameCamPos.x, gameCamPos.y, gameCamPos.z);
	vp.camSetRot(gameCamRot.x, gameCamRot.y, gameCamRot.z);

	al_clear_to_color(al_map_rgb(0, 0, 0));
	vp.render();
	int returnCode = windowManager.windowTick();
	
	return returnCode;

}
void Game::Start()
{
	windowManager.initWindow();
	vp.InitViewPort(windowManager.getSetWindow()[0], windowManager.getSetWindow()[1]);
	vp.InitGeometry();
}

int Game::Update() 
{
	input.tick();
	gameCamPos = gameCamPos + (Vector3(input.x, input.y, input.z) * moveMulti);
	gameCamRot = gameCamRot + Vector3(input.mx, input.my, 0.0f);
	
	vp.rotate(0, 0.05f, 0.0f, 0.0f);


	return renderPass(); //returns -1 on quit
}
