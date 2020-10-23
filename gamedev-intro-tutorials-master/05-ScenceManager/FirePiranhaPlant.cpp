#include "FirePiranhaPlant.h"
#include "Utils.h"
void FirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!INIT)
	{
		Startposy = y;
		SetState(FIREPIRANHAPLANT_HIDDEN);
		INIT = true;
	}

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
	
	if (Startposy - y >= 31)
		{
			if (IsAttack)
				SetState(FIREPIRANHAPLANT_ATTACK);
			vy = 0;
			if (!IsAttack && GetTickCount64() - AppearTime >= 3000)
			{
				SetState(FIREPIRANHAPLANT_HIDDEN);
				AppearTime = 0;
			}
			if (vy <= 0)
				CalcAtkzone();

		}
	if (Startposy - y <=-16 )
		{
			vy = 0;
			if (GetTickCount64() - AppearTime >= 3000)
			{
				AppearTime = 0;
				SetState(FIREPIRANHAPLANT_APPEAR);
			}
		}
	
	//CalcAtkzone();
	CGameObject::Update(dt);
	y += dy;
}

void FirePiranhaPlant::Render()
{
	if (EnemyX > x)
	{
		if (EnemyY < y)
			animation_set->at(0)->Render(x, y);
		else
			animation_set->at(1)->Render(x, y);
	}
	else
	{
		if (EnemyY < y)
			animation_set->at(3)->Render(x, y);
		else
			animation_set->at(2)->Render(x, y);
	}
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
		
	case FIREPIRANHAPLANT_ATTACK:
		CalcAtkPos();
		break;
	}
}
