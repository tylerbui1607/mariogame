#pragma once
#include "GameObject.h"
#define TAIL_BBOX_WIDH	16
#define TAIL_BBOX_HEIGHT	5

class Tail :
	public CGameObject
{
public:
	bool IsActivated;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state) {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		if (IsActivated)
		{
			left = x;
			top = y;
			right = left + TAIL_BBOX_WIDH;
			bottom = top + TAIL_BBOX_HEIGHT;
		}
	}
	void AdoptPos(float Mariox, float Marioy,int Marionx)
	{
		if (Marionx > 0)
			x = Mariox + 16;
		else
			x = Mariox - 16;
		y = Marioy + 18;
	}
};

