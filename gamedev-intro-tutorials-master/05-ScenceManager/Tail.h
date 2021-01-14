#pragma once
#include "GameObject.h"
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
	bool StopRender;
	bool IsActivated;
	bool GetBBox1;
	bool Walking,
		 Jumping,
		 Falling,
		 Attacking,
		 SlowFalling,
		 Idling;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state) {
		switch (state)
		{
		case TAIL_STATE_IDLING:
			Idling = true;
			SlowFalling = Falling = Walking = Jumping = false;
			StopRender = false;
			break;
		case TAIL_STATE_WALKING:
			Walking = true;
			SlowFalling = Falling = Idling = Jumping = false;
			break;
		case TAIL_STATE_FALLING:
			Falling = true;
			SlowFalling = Idling = Walking = Jumping = false;
			StopRender = false;
			break;
		case TAIL_STATE_JUMPING:
			Jumping = true;
			SlowFalling = Falling = Idling = Walking = false;
			break;
		case TAIL_STATE_ATTACKING:
			Attacking = true;
			SlowFalling = Falling = Idling = Walking = Jumping = false;
			break;
			
		}
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

