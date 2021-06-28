#pragma once
#include "GameObject.h"
#include "Effect.h"
#define TAIL_BBOX_WIDH	5
#define TAIL_BBOX_HEIGHT	5
#define TAIL_BBOX_ATKWIDH	14

#define TAIL_STATE_WALKING		100
#define TAIL_STATE_JUMPING		200
#define TAIL_STATE_FALLING		300
#define TAIL_STATE_SLOWFALLING	400
#define TAIL_STATE_ATTACKING	500
#define TAIL_STATE_IDLING		600


class Tail :
	public CGameObject
{
public:
	Tail()
	{
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(12);
		SetAnimationSet(ani_set);
		int ani = 0;
	};
	int ani;
	vector<Effect*> effects;
	bool StopRender;
	bool IsActivated;
	bool GetBBox1;
	int alpha =255;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state) {
		
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		if (IsActivated)
		{
			left = x;
			if(GetBBox1)
			{
				if (nx < 0)
					left = x - 28;
				else
					left = x + 19;
				right = left + TAIL_BBOX_ATKWIDH;
			}
			else {
				
				right = left + TAIL_BBOX_WIDH;
			}
			top = y;
			bottom = top + TAIL_BBOX_HEIGHT;
		}
	}
	void AdoptAtkpos(DWORD AtkTime)
	{
		if (GetTickCount64() - AtkTime >= 100 && GetTickCount64() - AtkTime <= 130)
		{
			GetBBox1 = true;
		}
		else
		{
			GetBBox1 = false;
		}
	}
	void AdaptMarioPos(float MarioX, float MarioY,int Marionx)
	{
		if (Marionx > 0)
		{
			x = MarioX - 5;
		}
		else
			x = MarioX + 14;
		y = MarioY + 18;
		nx = Marionx;
	}
};

