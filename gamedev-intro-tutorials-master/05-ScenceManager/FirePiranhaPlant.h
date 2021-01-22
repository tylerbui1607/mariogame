#pragma once
#include "GameObject.h"


#define	FIREPIRANHAPLANT_SPEED_Y	0.03

#define	FIREPIRANHAPLANT_MAXY_ATK	200
#define	FIREPIRANHAPLANT_MAXX_ATK	150
#define	FIREPIRANHAPLANT_MINX_ATK	16

#define FIREPIRANHAPLANT_APPEAR		2
#define FIREPIRANHAPLANT_HIDDEN		3
#define FIREPIRANHAPLANT_ATTACK		4

#define FIREPIRANHAPLANT_UPRIGHT	0
#define FIREPIRANHAPLANT_DOWNRIGHT	1
#define FIREPIRANHAPLANT_UPLEFT		3
#define FIREPIRANHAPLANT_DOWNLEFT	2
#include "FireBullet.h"
class FirePiranhaPlant :
	public CGameObject
{
public:
	float Startposy;
	DWORD CalcAtkTime;
	DWORD AppearTime;

	float AtkPosY;
	float StartAtkPosX;
	float StartAtkPosY;
	int count;
	float VxBullet;
	float VyBullet;

	float Max, Min;
	bool IsAttack,
		INIT;
	bool Appear, Hidden;
	bool StopMove;
	bool MarioNextTo;
	vector<FireBullet*>FireBullets;
	
public:
	FirePiranhaPlant() {};
	LPGAMEOBJECT mario;
	FirePiranhaPlant(float X, float Y)
	{
		ObjType = 8;
		StopMove = INIT = Appear = Hidden = IsAttack = false;
		IsMovingObject = true;
		x = X;
		y = Y;
		Max = y - 48;
		Min = y;
		SetState(FIREPIRANHAPLANT_APPEAR);
	}
	FirePiranhaPlant(float X, float Y,LPGAMEOBJECT m)
	{
		ObjType = 8;
		StopMove = INIT = Appear = Hidden = IsAttack  = false;
		IsMovingObject = true;
		x = X;
		y = Y;
		Max = y - 48;
		Min = y;
		mario = m;
		SetState(FIREPIRANHAPLANT_APPEAR);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	void CalcAtkzone()
	{
		if (  abs(x - EnemyX) <= FIREPIRANHAPLANT_MAXX_ATK && abs(y - EnemyY) <= FIREPIRANHAPLANT_MAXY_ATK)
		{
			if (!IsAttack)
				count = 1;
			IsAttack = true;
			SetState(FIREPIRANHAPLANT_ATTACK);
		}
		else
		{
			IsAttack = false;
		}
	}
	void CalcAtkPos()
	{
		if (abs(x - EnemyX) <= 100)
		{
			if (EnemyY < y)
				VyBullet = -0.1;
			else
				VyBullet = 0.1;
		}
		else
		{
			if (EnemyY < y)
				VyBullet = -0.08;
			else
				VyBullet = 0.08;
		}
		if (EnemyX > x)
		{
			VxBullet = 0.1;
			nx = 1;
		}
		else
		{
			VxBullet = -0.1;
			nx = -1;
		}
	}
	void CalcMarioNextTo()
	{
		if (EnemyX >= x - 25 && EnemyX <= x + 25)
			MarioNextTo = StopMove = true;
		else
			MarioNextTo=false;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		if (Health != 0)
		{
			left = x;
			top = y;
			right = left + 16;
			bottom = top + 32;
		}
	}
};

