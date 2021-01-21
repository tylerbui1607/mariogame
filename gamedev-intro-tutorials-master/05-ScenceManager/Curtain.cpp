#include "Curtain.h"
#include "Camera.h"
void Curtain::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	if (IsUp)
	{
		vy = -0.1;
	}
	if (IsDown)
	{
		if (AppearTime == 0)
			AppearTime = GetTickCount64();
		else {
			if (GetTickCount64() - AppearTime > 2500)
				vy = 0.23;
			if (GetTickCount64() - AppearTime > 6000)
				StopRender = true;
		}
	}
	if (y + 187 < 0)
		SubHealth();
	if (IsDown && y >= 0)
	{
		vy = 0; y = 0;
	}
	y += dy;
}

void Curtain::Render()
{
	if (!StopRender)
	{
		if (!choose)
		{
			animation_set->at(CURTAIN_ANIMATION)->Render(x, y);
		}
		else
			animation_set->at(1)->Render(x, y);

	}
}

void Curtain::SetState(int state)
{
}
