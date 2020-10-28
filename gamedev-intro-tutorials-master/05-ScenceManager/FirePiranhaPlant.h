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
	FireBullet* Firebullet;
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
		Firebullet = new FireBullet(0, 0);
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
		if (abs(x - EnemyX) <= 200)
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

