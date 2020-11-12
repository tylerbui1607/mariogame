#pragma once
#include "GameObject.h"
#include "Utils.h"
class FireBullet :
	public CGameObject
{
	
public:
	bool FireMario,
		 Disable;
	FireBullet() {
	}
	FireBullet(float Vx, float Vy)
	{
		DebugOut(L"CreatePublic\n");
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
		if (GetHealth() != 0)
		{
			left = x;
			top = y;
			right = left + 8;
			bottom = top + 9;
		}
	}
};

