#pragma once
#include "GameObject.h"
class Coin :
	public CGameObject
{
	float StartInitPos;
	bool Initialized;
public:
	Coin(int X, int Y)
	{
		ObjType = 18;
		IsMovingObject = true;
		Initialized = false;
		x = X;
		y = Y;
		StartInitPos = Y;

	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void CaclVx(int objx)
	{
		/*if (objx > x + MUSHROOOM_BBOX / 3)
			vx = -MUSHROOM_SPEED_X;
		else
			vx = MUSHROOM_SPEED_X;*/
	}
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
	}
};

