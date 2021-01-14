#include "FirePiranhaPlant.h"
#include "Utils.h"
#include "WarpPipe.h"
#define MAXHEIGHT_APPEAR	30
#define MAXHEIGHT_HIDDEN	16
void FirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (IsAttack && GetTickCount64() - CalcAtkTime >= 10000)
	{
		IsAttack = false;
	}
	if (IsAttack && GetTickCount64() - CalcAtkTime >= 2000)
	{
		IsAttack = false;
	}
	if (y <= Max)
	{
		 vy = 0;
		 y = Max + 0.01;
		 if (vy < 0)
			CalcAtkzone();
		if (!IsAttack)
		{
			SetState(FIREPIRANHAPLANT_HIDDEN);
		}
	}
	if ( y >= Min)
	{
		vy = 0;
		y = Min - 0.01;
		if (!StopMove)
		{
			SetState(FIREPIRANHAPLANT_APPEAR);
		}
	}
	if (IsAttack && count == 1)
	{
		CalcAtkPos();
		FireBullet*bullet = new FireBullet(VxBullet, VyBullet, x, y);
		FireBullets.push_back(bullet);
		count--;
	}
	for (int i = 0; i < FireBullets.size(); i++)
	{
		if (FireBullets.at(i)->Health != 0)
			FireBullets.at(i)->Update(dt, coObjects);
	}
	CGameObject::Update(dt);
	y += dy;
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->ObjType == ObjType::WARPPIPE)
		{
			if (CheckAABB(coObjects->at(i)))
			{
				WarpPipe* wp = dynamic_cast<WarpPipe*>(coObjects->at(i));
				StopMove = wp->MarioOn;
			}
		}
	}
}

void FirePiranhaPlant::Render()
{
	if (EnemyX > x)
	{
		if (EnemyY < y)
			animation_set->at(FIREPIRANHAPLANT_UPRIGHT)->Render(x, y);
		else
			animation_set->at(FIREPIRANHAPLANT_DOWNRIGHT)->Render(x, y);
	}
	else
	{
		if (EnemyY < y)
			animation_set->at(FIREPIRANHAPLANT_UPLEFT)->Render(x, y);
		else
			animation_set->at(FIREPIRANHAPLANT_DOWNLEFT)->Render(x, y);
	}
	for (int i = 0; i < FireBullets.size(); i++)
	{
		if (FireBullets.at(i)->Health != 0)
			FireBullets.at(i)->Render();
	}
	RenderBoundingBox();
}

void FirePiranhaPlant::SetState(int state)
{
	switch (state)
	{
	case FIREPIRANHAPLANT_APPEAR:
		Appear= false;
		vy = -FIREPIRANHAPLANT_SPEED_Y;
		break;
	case FIREPIRANHAPLANT_HIDDEN:
		Hidden = false;
		vy = FIREPIRANHAPLANT_SPEED_Y;
		CalcAtkTime = GetTickCount64();
		break;
	case FIREPIRANHAPLANT_ATTACK:
		CalcAtkTime = GetTickCount64();
		break;
	}
}
