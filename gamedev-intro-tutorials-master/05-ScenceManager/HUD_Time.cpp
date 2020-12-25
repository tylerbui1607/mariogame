#include "HUD_Time.h"

void HUD_Time::Update(DWORD dt)
{
	if (Count >= 1000)
	{
		LimitTime--;
		Count = 0;
	}
	else
	{
		Count += dt;
	}
}
