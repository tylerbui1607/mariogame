
#include "MushRoom.h"
#include "Utils.h"
void MushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	/*DebugOut(L"Y%f\n ", y);*/
	
	CGameObject::Update(dt);
	if (!Initialized)
	{
		if (StartInitPos - y <= MUSHROOOM_BBOX)
			y += dy;
		else
			SetState(MUSHROOM_STATE_NORMAL);
	}
	else
	{	
		vy += MUSHROOM_GRAVITY * dt;
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
}
void MushRoom::Render()
{
	animation_set->at(MUSHROOM_ANI)->Render(x, y);
	RenderBoundingBox();
}
	void MushRoom::SetState(int state)
	{
		switch (state)
		{
		case MUSHROOM_STATE_INIT:
			vy = -MUSHROOM_SPEED_y;
			break;	
		case MUSHROOM_STATE_NORMAL:
			vy = 0;
			Initialized = true;
			break;
		}
	}
