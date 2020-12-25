#include "Coin.h"

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (StartY - y >= 100)
		vy = -vy;
	else if (StartY - y <= 16 && vy > 0)
		SubHealth();
	CGameObject::Update(dt);
	y += dy;
}

void Coin::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Coin::SetState(int state)
{

}
