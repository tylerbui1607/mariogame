#include "Leaf.h"

void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (StartY - y >= 64)
	{
		vy = -vy;
	}
	CGameObject::Update(dt);
	y += dy;
}

void Leaf::Render()
{
	RenderBoundingBox();
}

void Leaf::SetState(int state)
{
	
}
