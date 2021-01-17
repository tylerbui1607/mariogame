#include "RedKoopas.h"
#include "Block.h"
#include "Brick.h"
#include "Ground.h"
void RedKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((MINx-GOOMBA_BBOX_WIDTH/2 >= x || x + KOOPAS_BBOX_WIDTH >= MAXx+ GOOMBA_BBOX_WIDTH / 2) && IsWalking)
	{
		vx = -vx;
		nx = -nx;
		if (nx < 0)
			x = MAXx - KOOPAS_BBOX_WIDTH - 2;
		else
			x = MINx+0.01;
	}
	CGameObject::Update(dt, coObjects);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (IsHidden && !IsReborning)
	{
		if (GetTickCount64() - TimeStartReborn >= 15000)
		{
			if (IsHolding || !IsAttack)
				SetState(KOOPAS_STATE_REBORN);
		}
	}
	if (IsReborning)
	{
		if (GetTickCount64() - TimeReborn >= 3600)
		{
			IsReborning = false;
			y -= 14;
			SetState(KOOPAS_STATE_WALKING);
		}
	}
	if (!IsHolding)
	{
		if (state != KOOPAS_STATE_DIE)
			vy += GRAVITY * dt;
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->ObjType == ObjType::GOOMBA)
				if (CheckAABB(coObjects->at(i))) {
					coObjects->at(i)->SetState(GOOMBA_STATE_DIE);
				}
		}
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

				if (e->obj->ObjType == ObjType::QUESTIONBRICK)
				{
					if (e->nx)
					{
						if (IsHidden)
							e->obj->SetState(BRICK_STATE_COLISSION);
					}
				}
				if (e->obj->ObjType == ObjType::GOOMBA)
				{
					if (e->nx && IsAttack)
					{
						e->obj->SetState(GOOMBA_STATE_DIE);
					}
				}
				if (e->obj->ObjType == ObjType::KOOPAS|| e->obj->ObjType == ObjType::REDKOOPAS)
				{
					if (e->nx && IsAttack)
					{
						e->obj->SetState(KOOPAS_STATE_DIE);
					}
				}
				if (e->ny && e->obj->ObjType == ObjType::BLOCK)
				{
					Block* block = dynamic_cast<Block*>(e->obj);
					MINx = block->x;
					MAXx = block->x + block->GetWidth();
				}
				if (e->ny && e->obj->ObjType == ObjType::GROUND)
				{
					Ground* ground = dynamic_cast<Ground*>(e->obj);
					MINx = ground->x;
					MAXx = ground->x + ground->GetWidth();
				}
				if (e->obj->ObjType == ObjType::BRICK)
				{
					if (e->ny < 0)
					{
						CBrick* brick = dynamic_cast<CBrick*>(e->obj);
						MINx = brick->x;
						MAXx = brick->x + BRICK_BBOX_WIDTH;
					}
					if (IsAttack && e->nx)
						e->obj->SubHealth();
				}
				if (e->nx && e->obj->ObjType != ItemType::MUSHROOM && e->obj->ObjType != ObjType::BLOCK)
				{
					vx = -vx;
				}
				if (e->nx && e->obj->ObjType == ObjType::BLOCK)
				{
					x += dx;
				}
				if (ny != 0) vy = 0;
				y += min_ty * dy + ny * 0.4f;
				if (e->ny < 0 && state == KOOPAS_STATE_DIEBYTAIL)
					vx = vy = 0;
			}
		}
	}
	else
	{
		x += dx;
	}
}
