#include "Leaf.h"

void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;


		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->nx && e->obj->ObjType != ObjType::BLOCK)
			{
				x += min_tx * dx + nx * 0.4f;
				vx = -vx;
			}
			x += dx;
			if (ny != 0) vy = 0;
			y += min_ty * dy + ny * 0.5f;
		}
	}
}

void Leaf::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Leaf::SetState(int state)
{
	switch (state)
	{
	case LEAF_STATE_INIT:
		StartY = y;
		vy = -0.15;
		break;
	}
}
