#include "FlyingWood.h"
#include "GameObject.h"
void FlyingWood::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	if (IsMarioOn)
	{
		if (DropTime == 0)
			DropTime = GetTickCount64();
		else if (DropTime != 0 && GetTickCount64() - DropTime >= 100)
		{
			vx = 0;
			vy = 0.07;
		}
	}
}

void FlyingWood::Render()
{
	animation_set->at(0)->Render(x, y);
}

void FlyingWood::SetState(int state)
{

}
