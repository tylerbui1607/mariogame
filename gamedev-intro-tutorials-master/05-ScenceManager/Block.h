#pragma once
#include "GameObject.h"
class Block :
	public CGameObject
{
	int Width;
	int Height;
public:
	Block(int W, int H)
	{
		Width = W;
		Height = H;
		ObjType = 6;
	}
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + Width;
		bottom = top + Height;
	}
	virtual void Render() {
		RenderBoundingBox();
	}
};

