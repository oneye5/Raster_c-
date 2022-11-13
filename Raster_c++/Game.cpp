#include "Game.h"
#include "3dGameMethods.h"
#include"renderer.h"
#include<windows.system.threading.h>

ViewPort vp = ViewPort();
vector3 gameCamPos = vector3(0.0f,-4.0f,10.0f);
vector3 gameCamRot = vector3(0.0f,-90.0f,0.0f);
Input input = Input();
vector3 moveMulti = vector3(0.005f, 0.005f, 0.005f);
float fps=0.0f;

int renderPass()
{
	vp.camSetPos(gameCamPos.x, gameCamPos.y, gameCamPos.z);
	vp.camSetRot(gameCamRot.x, gameCamRot.y, gameCamRot.z);

	al_clear_to_color(al_map_rgb(0, 0,50));
	vp.render();
	int returnCode = windowManager.windowTick();


	

	if (returnCode != -1 && input.escape)
		returnCode = -1;
	return returnCode;

}
void playerControls(float& deltaTime)
{
	input.tick(windowManager.getDisplay());	
	gameCamRot = gameCamRot + vector3(input.my, -input.mx, 0.0f);
	clipRot(gameCamRot);

	vector3 forward = camForward(gameCamRot);
	vector3 right = camRight(gameCamRot);

	
	vector3 x = right * input.x ;
	vector3 y = vector3(0.0f, input.y, 0.0f);
	vector3 z = forward * input.z;

	x = x * deltaTime;
	y = y * deltaTime;
	z = z * deltaTime;

	x = x * moveMulti;
	y = y * moveMulti;
	z = z * moveMulti;

	gameCamPos = gameCamPos +z - x - y;
}

void Game::Start()
{
	windowManager.initWindow();
	vp.InitViewPort(windowManager.getSetWindow()[0], windowManager.getSetWindow()[1]);
	vp.InitGeometry();

	al_hide_mouse_cursor(windowManager.getDisplay());
	al_set_window_title(windowManager.getDisplay(), "press esc to exit");

	
}

int Game::Update(float deltaTime) 
{
	fps = (1.0f / deltaTime) * 1000.0f;
	playerControls(deltaTime);
	
	

	//vp.setRotation(1,gameCamRot.x * 4.0f,gameCamRot.y * 4.0f,gameCamRot.z * 4.0f);
//	vp.setLightPos(4, gameCamPos.x, gameCamPos.y, gameCamPos.z);
	
	//std::cout << fps << "\n";
	//std::cout << gameCamPos.x << "f , " << gameCamPos.y << "f , " << gameCamPos.z << "f\n"; // I USE THIS TO PLACE LIGHTS AND GET LOCATION
	return renderPass(); //returns -1 on quit
}
