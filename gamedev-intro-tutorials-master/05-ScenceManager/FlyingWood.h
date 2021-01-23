#pragma once
#include "GameObject.h"
class FlyingWood :
	public CGameObject
{
public:
	bool IsMarioOn;
	DWORD DropTime;
	FlyingWood()
	{
		IsMovingObject = true;
		vx = -0.02;
		ObjType = ObjType::MOVINGWOOD;
		DropTime = 0;
	};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = x;
		top = y;
		right = left + 48;
		bottom = top + 16;
	};
	virtual void SetState(int state);
};

