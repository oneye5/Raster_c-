#pragma once
#include <windows.h>  
#include "windowManager.h"
#include"allegro5/allegro5.h"
#include <iostream>
class Game
{
public:
	void Start();
	int Update();

};
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
struct Vector3
{
public:
	float x;
	float y;
	float z;
	Vector3(float X = 0, float Y = 0, float Z = 0)
	{
		x = X;
		y = Y;
		z = Z;
	}

	friend Vector3 operator+(const Vector3& v1, const Vector3& v2)
	{
		float x = v1.x + v2.x;
		float y = v1.y + v2.y;
		float z = v1.z + v2.z;
		return Vector3(x, y, z);
	}
	friend Vector3 operator-(const Vector3& v1, const Vector3& v2)
	{
		float x = v1.x - v2.x;
		float y = v1.y - v2.y;
		float z = v1.z - v2.z;
		return Vector3(x, y, z);
	}
	friend Vector3 operator*(const Vector3& v1, const Vector3& v2)
	{
		float x = v1.x * v2.x;
		float y = v1.y * v2.y;
		float z = v1.z * v2.z;
		return Vector3(x, y, z);
	}
	friend Vector3 operator*(const Vector3& v1, const float v2)
	{
		float x = v1.x * v2;
		float y = v1.y * v2;
		float z = v1.z * v2;
		return Vector3(x, y, z);
	}
	friend Vector3 operator/(const Vector3& v1, const float v2)
	{
		float x = v1.x / v2;
		float y = v1.y / v2;
		float z = v1.z / v2;
		return Vector3(x, y, z);
	}
};