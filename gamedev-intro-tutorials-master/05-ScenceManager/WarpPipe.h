#pragma once
#include "GameObject.h"
#define TileSize	16

#define Ani_Start_Right		0
#define Ani_Start_LEFT		1
#define Ani_Body_Right		2
#define Ani_Body_Left		3

class WarpPipe : 
	public CGameObject
{
	int Width;
	int Height;
public:
	WarpPipe(int W, int H)
	{
		Width = W;
		Height = H;
		ObjType = 5;
	}
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + Width;
		bottom = top + Height;
	}
	virtual void Render();
};

