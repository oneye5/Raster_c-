#include "Game.h"
#include "windowManager.h"
#include"renderer.h"
#include<windows.system.threading.h>

ViewPort vp = ViewPort();
void Game::Start()
{
	windowManager.initWindow();
	vp.InitViewPort(windowManager.getSetWindow()[0], windowManager.getSetWindow()[1]);
	vp.InitGeometry();
}

int Game::Update() //return -1 to quit
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	vp.render();
	int returnCode = windowManager.windowTick();
	vp.rotate(0, 0.05f, 0.25f, 0.025f);

	return returnCode;
}
