#include "Goomba.h"
CGoomba::CGoomba()
{
	IsMovingObject = true;
	SetState(GOOMBA_STATE_WALKING);
	ObjType = ObjType::GOOMBA;
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!IsDying)
	{
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;

		if (state == GOOMBA_STATE_DIE)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (IsDying && GetTickCount64() - DyingTime >= 1500)
	{
		SubHealth();
	}
	if (!IsDying)
	{
		if (state != GOOMBA_STATE_DIE)
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
					vx = -vx;
				}
				x += dx;
				if (ny != 0) vy = 0;
				y += min_ty * dy + ny * 0.5f;
			}
		}
	}
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}

	animation_set->at(ani)->Render(x,y);

	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			if (!IsDying)
			{
				y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
				vx = 0;
				vy = 0;
				IsDying = true;
				DyingTime = GetTickCount64();
			}
			break;
		case GOOMBA_STATE_WALKING: 
			vx = GOOMBA_WALKING_SPEED;
			break;
	}
}
