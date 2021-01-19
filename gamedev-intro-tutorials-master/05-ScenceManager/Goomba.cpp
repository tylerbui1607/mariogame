#include "Goomba.h"
CGoomba::CGoomba()
{
	IsMovingObject = true;
	SetState(GOOMBA_STATE_WALKING);
	ObjType = ObjType::GOOMBA;
	Level = 1;
}

CGoomba::CGoomba(int level)
{
	IsMovingObject = true;
	nx = 1;
	ObjType = ObjType::GOOMBA;
	Level = level;
	count = 0;
	SetState(GOOMBA_STATE_WALKING);

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
	if (state==GOOMBA_STATE_DIE && GetTickCount64() - DyingTime >= 1000)
	{
		SubHealth();
	}
	if (IsFlyGoomba && GetTickCount64() - WalkingTime >= 1000)
	{
		SetState(GOOMBA_STATE_READY_FLY);
		IsFlyGoomba = false;
	}
	if (state != GOOMBA_STATE_DIE)
	{
		if (state != GOOMBA_STATE_DIE)
			vy += GRAVITY * dt;
	if (ReadyFly && count == 0)
		SetState(GOOMBA_STATE_FLY);
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
					this->nx *= -1;
					vx = -vx;
				}
				x += dx;
				if (ny != 0) vy = 0; 
				if (ReadyFly && count != 0)
				{
					if (e->ny < 0)
					{
						vy = -0.05;
						count--;
					}
				}
				
				if (IsFlying)
				{
					if (e->ny < 0)
						SetState(GOOMBA_STATE_WALKING);
				}
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
			vx = nx*GOOMBA_WALKING_SPEED;
			if (Level == GOOMBA_LEVEL_FLY)
			{
				IsFlyGoomba = true;
				WalkingTime = GetTickCount64();
				IsFlying = false;
			}
			break;
		case GOOMBA_STATE_DIEBYTAIL:
			IsDying = true;
			vy = -0.25;
			vx = 0.1 * nx;
			break;
		case GOOMBA_STATE_FLY:
			IsFlying = true;
			ReadyFly = false;
			vy = -0.2;
			break;
		case GOOMBA_STATE_READY_FLY:
			ReadyFly = true;
			count = 3;
			break;
	}
}
