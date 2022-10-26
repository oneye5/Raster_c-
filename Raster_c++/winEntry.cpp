#include "winEntry.h"
#include <iostream>
#include<windows.system.threading.h>
int main()
{
	auto game = Game();
	game.Start();
	bool running = true;
	while (running)
	{
		if (game.Update() == -1)
			running = false;
		///Sleep(1);
	}
}