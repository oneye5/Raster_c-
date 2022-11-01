#include "windowManager.h"
#include <iostream>

int WindowWidth = 1600;
int WindowHeight = 900;

bool running = true;
ALLEGRO_DISPLAY* Display;
ALLEGRO_EVENT_QUEUE* Queue;


void WindowManager::initWindow()
{
	al_init();
	al_init_primitives_addon();
	al_init_image_addon();
	Queue = al_create_event_queue();
	
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	Display = al_create_display(WindowWidth, WindowHeight);
	al_install_keyboard();
	al_install_mouse();
	al_grab_mouse(Display);

	al_register_event_source(Queue, al_get_keyboard_event_source());
	al_register_event_source(Queue, al_get_display_event_source(Display));
	
}


int WindowManager::windowTick()
{
#pragma endregion
	al_flip_display();
		//event handeling
#pragma region eventHandler
			
			ALLEGRO_EVENT Event;
			if (!al_is_event_queue_empty(Queue))
			{
				al_wait_for_event(Queue, &Event);
				if (Event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
					running = false;
			}
#pragma endregion
			if (running == true)
				return 1;
	al_destroy_display(Display);
	al_uninstall_keyboard();
	al_ungrab_mouse();
	al_uninstall_mouse();
	al_shutdown_image_addon();
	return -1;
}

std::vector<int> WindowManager::getSetWindow(int setw,int seth)
{
	return std::vector<int>{WindowWidth,WindowHeight};
}

ALLEGRO_DISPLAY* WindowManager::getDisplay()
{
	return Display;
}
