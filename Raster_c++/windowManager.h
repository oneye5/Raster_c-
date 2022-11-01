#pragma once
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include <vector>
#define WINDOWMANAGER
static class WindowManager
{
public:
	void initWindow();
	int windowTick();
	std::vector< int> getSetWindow(int setw = -1, int seth = -1);
	ALLEGRO_DISPLAY* getDisplay();
}windowManager;