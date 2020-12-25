#include "Hud.h"

void Hud::Update(DWORD dt)
{
	hudTime->Update(dt);
}

void Hud::Render()
{
	CSprites::GetInstance()->Get(SpriteHUD)->DrawHUD(HUDx, HUDy);
	hudTime->Render();
}
