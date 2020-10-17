#pragma once
#include "GameObject.h"
class Block :
	public CGameObject
{
	int Height;
	int Width;
public:
	Block(int W,int H)
	{
		Width = W;
		Height = H;
	}
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + Width;
		bottom = top + Height;
	}
};

