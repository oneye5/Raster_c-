#pragma once
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro.h"
#define WINDOWMANAGER
static class WindowManager
{
public:
	void initWindow();
	void initRenderer();
	void windowLoop();
}windowManager;