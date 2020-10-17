#pragma once
#include "GameObject.h"
#define BBOX_WIDTH	32
class WarpPipe :
	public CGameObject
{
	int Height;
public:
	WarpPipe( int H)
	{
		Height = H;
	}
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + BBOX_WIDTH;
		bottom = top + Height;
	}
};

