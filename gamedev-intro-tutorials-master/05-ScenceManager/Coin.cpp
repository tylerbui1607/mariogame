#include "Coin.h"

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (StartY - y >= 100)
		vy = -vy;
	else if (StartY - y <= 16 && vy > 0 && !InnitEffect)
	{
		effect = new Effect(x, y, 40, 0);
		effect->SetState(40);
		InnitEffect = true;
	}
	if (InnitEffect)
	{
		effect->Update(dt);
		if (effect->StopRender)
			SubHealth();
	}
	CGameObject::Update(dt);
	y += dy;
}

void Coin::Render()
{
	if (InnitEffect)
	{
		effect->Render();
	}
	else
		animation_set->at(0)->Render(x, y);
}

void Coin::SetState(int state)
{

}
