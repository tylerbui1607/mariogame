#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Utils.h"

#define BULLET_ANI_SET	13

#define BULLET_ANI_FLY_RIGHT	0
#define BULLET_ANI_FLY_LEFT		1
class FireBullet :
	public CGameObject
{
	
public:
	bool FireMario,
		 Disable;
	bool Attack;
	FireBullet()
	{
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(BULLET_ANI_SET);
		SetAnimationSet(ani_set);
	};
	FireBullet(float X, float Y) {
		x = X;
		y = Y;
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(BULLET_ANI_SET);
		SetAnimationSet(ani_set);
	}
	FireBullet(float Vx, float Vy, float X, float Y)
	{
		x = X;
		y = Y;
		vx = Vx;
		vy = Vy;
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(BULLET_ANI_SET);
		SetAnimationSet(ani_set);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render()
	{
		if (vx > 0)
			animation_set->at(BULLET_ANI_FLY_RIGHT)->Render(x, y);
		else
			animation_set->at(BULLET_ANI_FLY_LEFT)->Render(x, y);
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

