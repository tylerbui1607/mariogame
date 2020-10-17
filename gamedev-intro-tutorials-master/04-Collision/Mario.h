#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED			0.15f 
//0.1f
#define MARIO_JUMP_SPEED_Y			0.15f
#define MARIO_JUMP_DEFLECT_SPEED	0.2f
#define MARIO_GRAVITY				0.002f
#define MARIO_DIE_DEFLECT_SPEED		0.5f
#define MARIO_ACCLERATION			0.004f
#define MARIO_MAX_SPEED				0.15f
#define MARIO_FRICTION				0.004f
#define MARIO_FLY_SPEED				0.25f
#define MARIO_MAX_HIGH_JUMP			70

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_SIT				500
#define MARIO_STATE_RUN				600
#define MARIO_STATE_FLY				700
#define MARIO_STATE_FALLING         800



#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7

#define MARIO_ANI_DIE						8
#define MARIO_RB_WALKINGRIGHT				9
#define MARIO_RB_WALKINGLEFT				10
#define MARIO_ANI_JUMPINGRIGHT				11
#define MARIO_ANI_JUMPINGLEFT				12
#define MARIO_ANI_FALLINGRIGHT				13
#define MARIO_ANI_FALLINGLEFT				14
#define MARIO_ANI_SITTINGRIGHT				15
#define MARIO_ANI_SITTINGLEFT				16
#define MARIO_ANI_SMALLJUMPRIGHT			17
#define MARIO_ANI_SMALLJUMPLEFT				18
#define MARIO_ANI_FASTESTRUNRIGHT			19
#define MARIO_ANI_FASTESTRUNLEFT			20	




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACOON	3

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	int CounterSpeed;
	DWORD untouchable_start;
	float StartJump;
public:
	bool IsOnPlatForm,
		 IsRollBack,
		 IsSitting,
		 IsRunning,
		 IsFlying,
		 IsFalling;

	int ani;
	DWORD TRollBack = 0;
	DWORD TPlusStack = 0;
	DWORD TCanFly = 0;
public: 
	CMario() : CGameObject()
	{
		CounterSpeed = 0;
		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		IsFalling = IsFlying = IsRunning = IsSitting = IsRollBack = IsOnPlatForm = false;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StopRunning()
	{
		IsRunning = false;
	}
	void IncreaseStack()
	{
		if (CounterSpeed < 7 && (GetTickCount64() - TPlusStack >= 200))
		{
			TPlusStack = 0;
			CounterSpeed++;
		}
	}
	void DecreaseStack()
	{
		if (CounterSpeed >0 && (GetTickCount64() - TPlusStack >= 200))
		{
			TPlusStack = 0;
			CounterSpeed--;
		}
	}
	void CalStartJumpPosition()
	{
		StartJump = y;
	}
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};