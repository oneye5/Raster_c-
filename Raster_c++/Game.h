#pragma once
#include <windows.h>  
#include "windowManager.h"
#include"allegro5/allegro5.h"
#include <iostream>

struct Input
{
	float x = 0.0f;
	float y = 0.0f; 
	float z = 0.0f;

	float mx = 0.0f;
	float my = 0.0f;
	bool mDown = false;

	

	int mousePosX = 0;
	int mousePosY = 0;
	void tick(ALLEGRO_DISPLAY* d)
	{
		float deltaMouseDiv = 10.0f;
		int newMousePosX;
		int newMousePosY;

		int* tx = &newMousePosX;
		int* ty = &newMousePosY;
		al_get_mouse_cursor_position(tx, ty);
		
		mx = newMousePosX - mousePosX;
		my = newMousePosY - mousePosY;



		int setx = al_get_display_width(d) / 2.0f;
		int sety = al_get_display_height(d) / 2.0f;
		SetPhysicalCursorPos(setx,sety);

		mx /= deltaMouseDiv;
		my /= deltaMouseDiv;


		mousePosX = setx;
		mousePosY = sety;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;

		mDown = false;

		if (GetKeyState('W') < 0)
			z++;
		if (GetKeyState('S') < 0)
			z--;
		if (GetKeyState('A') < 0)
			x--;
		if (GetKeyState('D') < 0)
			x++;

		if (GetKeyState(VK_SPACE) < 0)
			y++;
		if (GetKeyState(VK_CONTROL) < 0)
			y--;
	}
};
class Game
{
public:
	void Start();
	int Update(float deltaTime);

};





/*
void g_rayFromAngle(vector2 A,vector3& out)
{
	vector3 pos = vector3();

	sinf(degToRad(A.y))* cosf(degToRad(A.x));
	cosf(degToRad(A.x));
	cosf(degToRad(A.y))* cosf(degToRad(A.x));
}

vector3 camForward(vector3 v)
{
	//convert to radians

	v.x = degToRad(v.x);
	v.y = degToRad(v.y);
	v.z = degToRad(v.z);

	vector3 pos = vector3();
	g_rayFromAngle(v,pos);
	return pos;
}
vector3 camRight(vector3 v)
{
	//convert to radians
	v.y = v.y + 90.0f;

	v.x = degToRad(v.x);
	v.y = degToRad(v.y);
	v.z = degToRad(v.z);

	vector3 pos;
	g_rayFromAngle(v,pos);
	return pos;
}
*/
