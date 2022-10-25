#pragma once

class ViewPort
{
public:
	//public methods

	void InitViewPort(int ScreenWidth, int ScreenHeight);
	void InitGeometry();

	void render();
};