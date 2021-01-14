#pragma once
#include "GameObject.h"
class Ground :
	public CGameObject
{
	int Width;
	int Height;
public:
	Ground(int W, int H)
	{
		Width = W;
		Height = H;
		ObjType = 4;
	}
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + Width;
		bottom = top + Height;
	}
	virtual int GetWidth() { return Width; };

	virtual void Render();
};

