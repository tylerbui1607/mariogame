#include "FirePiranhaPlant.h"
#include "Utils.h"
#include "WarpPipe.h"
#define MAXHEIGHT_APPEAR	30
#define MAXHEIGHT_HIDDEN	16
void FirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (IsAttack && GetTickCount64() - CalcAtkTime >= 2000)
	{
		IsAttack = false;
	}
	if (Appear && GetTickCount64() - AppearTime > 3000)
	{
		Appear = false;
	}
	if (Hidden && GetTickCount64() - AppearTime > 3000)
	{
		Hidden = false;
	}
	if (y <= Max)
	{
		 y = Max;
		 if (vy < 0)
			CalcAtkzone();
		 vy = 0;
		if (!IsAttack && !Appear)
		{
			SetState(FIREPIRANHAPLANT_HIDDEN);
		}
	}
	if ( y >= Min)
	{
		vy = 0;
		y = Min;
		if (!StopMove && !Hidden)
		{
			SetState(FIREPIRANHAPLANT_APPEAR);
		}
	}
	if (IsAttack && count == 1)
	{
		FireBullet* bullet;
		CalcAtkPos();
		if (nx < 0)
			bullet = new FireBullet(VxBullet, VyBullet, x, y);
		else
			bullet = new FireBullet(VxBullet, VyBullet, x+8, y);
		FireBullets.push_back(bullet);
		coObjects->push_back(bullet);
		count--;
	}
	for (int i = 0; i < FireBullets.size(); i++)
	{
		if (FireBullets.at(i)->Health != 0)
			FireBullets.at(i)->Update(dt, coObjects);
	}
	CalcMarioNextTo();
	CGameObject::Update(dt);
	y += dy;
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->ObjType == ObjType::WARPPIPE)
		{
			if (CheckAABB(coObjects->at(i)))
			{
				WarpPipe* wp = dynamic_cast<WarpPipe*>(coObjects->at(i));
				if(!MarioNextTo)
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
		Appear = true;
		AppearTime = GetTickCount64();
		vy = -FIREPIRANHAPLANT_SPEED_Y;
		break;
	case FIREPIRANHAPLANT_HIDDEN:
		Hidden = true;
		AppearTime = GetTickCount64();
		vy = FIREPIRANHAPLANT_SPEED_Y;
		CalcAtkTime = GetTickCount64();
		break;
	case FIREPIRANHAPLANT_ATTACK:
		CalcAtkTime = GetTickCount64();
		break;
	}
}
