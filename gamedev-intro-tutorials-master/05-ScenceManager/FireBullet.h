#pragma once
#include "GameObject.h"
class FireBullet :
	public CGameObject
{
public:
	FireBullet(float Vx, float Vy)
	{
		vx = Vx;
		vy = Vy;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render()
	{
		RenderBoundingBox();
	}
	void SetState(int state) {}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
			left = x;
			top = y;
			right = left + 8;
			bottom = top + 9;
	}
};

