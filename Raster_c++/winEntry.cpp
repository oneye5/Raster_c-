#include "winEntry.h"
#include <iostream>
#include<windows.system.threading.h>
#include <conio.h>  
#include<chrono>
#include<time.h>
int main()
{
	auto game = Game();
	game.Start();
	bool running = true;
	float deltaTime = 0;
	while (running)
	{
		auto  startTime = std::chrono::high_resolution_clock::now();
		if (game.Update(deltaTime) == -1)
			running = false;
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		deltaTime = duration.count();
	}
}