#include "PiranhaPlant.h"
#include "WarpPipe.h"
void PiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
		vy = 0;
		if (!Appear)
		{
			SetState(FIREPIRANHAPLANT_HIDDEN);
		}
	}
	if (y >= Min)
	{
		vy = 0;
		y = Min;
		if (!StopMove && !Hidden)
		{
			SetState(FIREPIRANHAPLANT_APPEAR);
		}
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
				if (!MarioNextTo)
					StopMove = wp->MarioOn;

			}
		}
	}
}

void PiranhaPlant::Render()
{
	animation_set->at(0)->Render(x, y);
}

void PiranhaPlant::SetState(int state)
{
	FirePiranhaPlant::SetState(state);
}
