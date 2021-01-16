#pragma once
#include "FirePiranhaPlant.h"
class PiranhaPlant :
	public FirePiranhaPlant
{
public:
	PiranhaPlant(float X, float Y)
	{
		ObjType = 8;
		INIT = IsAttack = IsMovingObject = false;
		x = X;
		y = Y;
		Max = y - 24;
		Min = y + 8;
		SetState(FIREPIRANHAPLANT_APPEAR);
	};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		if (Health != 0)
		{
			left = x;
			top = y;
			right = left + 16;
			bottom = top + 24;
		}
	}
	void SetState(int state);
};

