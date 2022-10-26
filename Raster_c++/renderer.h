#pragma once
#include<vector>


class ViewPort
{
public:
	//public methods
	void InitViewPort(int ScreenWidth, int ScreenHeight);
	void InitGeometry();

	void render();

	void translate(int index, float x, float y, float z);
	void setPosition(int index, float x, float y, float z);
	void rotate(int index, float x, float y, float z);
	void setRotation(int index, float x, float y, float z);
};