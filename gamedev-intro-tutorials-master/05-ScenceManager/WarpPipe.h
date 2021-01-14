#pragma once
#include "GameObject.h"
#define TileSize	16

#define Ani_Start_Right		0
#define Ani_Start_LEFT		1
#define Ani_Body_Right		2
#define Ani_Body_Left		3

#define WARPIPE_STATE_HAVE_MARIOON	1
#define WARPIPE_STATE_NOTHAVE_MARIOON	2
class WarpPipe : 
	public CGameObject
{
	int Width;
	int Height;
public:
	bool MarioOn;
	WarpPipe() {};
	WarpPipe(int W, int H)
	{
		Width = W;
		Height = H;
		ObjType = ObjType::WARPPIPE;
		MarioOn = false;
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
	virtual void SetState(int state)
	{
		switch (state)
		{
		case WARPIPE_STATE_HAVE_MARIOON:
			MarioOn = true;
			break;
		case WARPIPE_STATE_NOTHAVE_MARIOON:
			MarioOn = false;
			break;
		}
	}
};

