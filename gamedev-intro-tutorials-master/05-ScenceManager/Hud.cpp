#include "Hud.h"
Hud* Hud::__instance = NULL;
void Hud::Update(DWORD dt)
{
	if (StopTime)
	{
		hudTime->LimitTime = 0;
	}
	else
		hudTime->Update(dt);
	if (CourseClear)
	{
		if (Time == 0)
			Time = GetTickCount64();
		else if (Time != 0 && GetTickCount64() - Time > 2000)
			CourseClear = false;
	}
}

void Hud::Render(int Score)
{
	CSprites::GetInstance()->Get(SpriteHUD)->DrawHUD(HUDx, HUDy);
	for (int i = 1; i <= MarioStack; i++)
	{
		if (i < 7)
			CSprites::GetInstance()->Get(SpriteStack)->DrawHUD(HUDx + 58 + 8 * (i - 1), HUDy+9);
		else
			CSprites::GetInstance()->Get(SpritePower)->DrawHUD(HUDx + 58 + 8 * (i - 1), HUDy + 9);
	}
	if (!StopTime)
	{
		for (int i = 0; i < Item.size(); i++)
		{
			CSprites::GetInstance()->Get(400 + Item[i])->DrawHUD(HUDx + 170 + 24 * i, HUDy + 7);
		}
	}
	if (CourseClear)
	{
		CSprites::GetInstance()->Get(404)->DrawHUD(60, 32);
	}
	hudTime->Render(Score,MarioMoney);
}
