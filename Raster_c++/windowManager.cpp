#include "windowManager.h"


int WindowWidth = 1000;
int WindowHeight = 500;

bool running = true;
ALLEGRO_DISPLAY* Display;
ALLEGRO_EVENT_QUEUE* Queue;
void WindowManager::initWindow()
{
	al_init();
	al_init_primitives_addon();
	Queue = al_create_event_queue();;
	Display = al_create_display(WindowWidth, WindowHeight);
	al_install_keyboard();

	al_register_event_source(Queue, al_get_keyboard_event_source());
	al_register_event_source(Queue, al_get_display_event_source(Display));
}
void WindowManager::initRenderer()
{

}
void Render()
{
	al_draw_pixel(0,0, al_map_rgb(255,255,255));
}
void WindowManager::windowLoop()

{
#pragma endregion
	do
	{
		Render();
		al_flip_display();

		//event handeling
#pragma region eventHandler
			
			ALLEGRO_EVENT Event;
			al_wait_for_event(Queue, &Event);
			if (Event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				running = false;
			
#pragma endregion
	} while (running);
	al_destroy_display(Display);
	al_uninstall_keyboard();
}