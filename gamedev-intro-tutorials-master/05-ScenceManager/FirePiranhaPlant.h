#pragma once
#include "GameObject.h"


#define	FIREPIRANHAPLANT_SPEED_Y	0.03

#define	FIREPIRANHAPLANT_MAXY_ATK	64
#define	FIREPIRANHAPLANT_MAXX_ATK	112
#define	FIREPIRANHAPLANT_MINX_ATK	16

#define FIREPIRANHAPLANT_APPEAR		2
#define FIREPIRANHAPLANT_HIDDEN		3
#define FIREPIRANHAPLANT_ATTACK		4

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

	bool IsAttack,
		INIT;
public:
	FirePiranhaPlant()
	{
		ObjType = 8;
		INIT = IsAttack = IsMovingObject = false;
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
		}
		else
		{
			IsAttack = false;
		}
	}
	void CalcAtkPos()
	{
		if (abs(x - EnemyX) <= 64)
		{
			if (EnemyY < y)
				VyBullet = -0.03;
			else
				VyBullet = 0.03;
		}
		else
		{
			if (EnemyY < y)
				VyBullet = -0.01;
			else
				VyBullet = 0.01;
		}
		if (EnemyX > x)
		{
			VxBullet = 0.03;
			nx = 1;
		}
		else
		{
			VxBullet = -0.03;
			nx = -1;
		}
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

