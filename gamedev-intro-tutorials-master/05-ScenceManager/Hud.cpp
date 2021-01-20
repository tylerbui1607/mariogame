#include "Hud.h"
Hud* Hud::__instance = NULL;
void Hud::Update(DWORD dt)
{
	hudTime->Update(dt);
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
	hudTime->Render(Score,MarioMoney);
}
