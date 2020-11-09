#include "FirePiranhaPlant.h"
#include "Utils.h"
#define MAXHEIGHT_APPEAR	31
#define MAXHEIGHT_HIDDEN	16

void FirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Intiate FirePiranhaPlant
	if (!INIT)
	{
		Startposy = y;
		SetState(FIREPIRANHAPLANT_HIDDEN);
		INIT = true;
	}
	//Calculate atk time
	if (IsAttack && CalcAtkTime == 0)
	{
		CalcAtkTime = GetTickCount64();
	}
	if (IsAttack && GetTickCount64() - CalcAtkTime >= 2000)
	{
		IsAttack = false;
		CalcAtkTime = 0;
	}
	if ( AppearTime == 0)
	{
		AppearTime = GetTickCount64();
	}
	
	if (Startposy - y >= MAXHEIGHT_APPEAR)
		{
			vy = 0;
			if (!IsAttack && GetTickCount64() - AppearTime >= 3000)
			{
				SetState(FIREPIRANHAPLANT_HIDDEN);
				AppearTime = 0;
			}
			if (vy <= 0)
				CalcAtkzone();
		}
	if (Startposy - y <=-MAXHEIGHT_HIDDEN )
		{
			vy = 0;
			if (GetTickCount64() - AppearTime >= 3000)
			{
				AppearTime = 0;
				SetState(FIREPIRANHAPLANT_APPEAR);
			}
		}
	if (IsAttack && count == 1)
	{
		CalcAtkPos();
		Firebullet = new FireBullet(VxBullet, VyBullet);
		Firebullet->SetPosition(x,y);
		count--;
	}
	Firebullet->Update(dt,coObjects);
	CGameObject::Update(dt);
	y += dy;
	//DebugOut(L"hello%f\n", VyBullet);
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
	Firebullet->Render();
	RenderBoundingBox();
}

void FirePiranhaPlant::SetState(int state)
{
	switch (state)
	{
	case FIREPIRANHAPLANT_APPEAR:
		vy = -FIREPIRANHAPLANT_SPEED_Y;
		break;
	case FIREPIRANHAPLANT_HIDDEN:
		vy = FIREPIRANHAPLANT_SPEED_Y;
	}
}
