#include "RedKoopas.h"
#include "Block.h"
#include "Brick.h"
#include "Ground.h"
void RedKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (!IsPara)
	{
		if (state == KOOPAS_STATE_WALKING)
		{
			if (nx > 0)
			{
				KPAI->SetPosition(x + 10, y);
				if (KPAI->isFalling)
				{
					nx = -1;
					KPAI->SetPosition(x - 15, y);
					SetState(KOOPAS_STATE_WALKING);
				}
			}
			else
			{
				KPAI->SetPosition(x - 10, y);
				if (KPAI->isFalling)
				{
					nx = 1;
					KPAI->SetPosition(x + 5, y);
					SetState(KOOPAS_STATE_WALKING);
				}
			}
		}
		if (KPAI != NULL)
			KPAI->Update(dt, coObjects);
		if (IsHidden && !IsReborning)
		{
			if (GetTickCount64() - TimeStartReborn >= 5000)
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
						coObjects->at(i)->nx = nx;
						coObjects->at(i)->SetState(GOOMBA_STATE_DIEBYTAIL);
					}
				if (coObjects->at(i)->ObjType == ObjType::QUESTIONBRICK)
					if (CheckAABB(coObjects->at(i))) {
						SetState(KOOPAS_STATE_DIEBYTAIL);
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
							if (e->nx != 0)
							{
								if (e->obj->y < y)
								{
									this->nx = -this->nx;
									vx = -vx;
								}
								else
									x += dx;
							}
							if (IsHidden)
								e->obj->SetState(BRICK_STATE_COLISSION);
						}
					}
					if (e->obj->ObjType == ObjType::GOOMBA)
					{
						if (e->nx && IsAttack)
						{
							e->obj->SetState(GOOMBA_STATE_DIEBYTAIL);
						}
					}
					if (e->obj->ObjType == ObjType::KOOPAS || e->obj->ObjType == ObjType::REDKOOPAS)
					{
						if (e->nx && IsAttack)
						{
							e->obj->SetState(KOOPAS_STATE_DIEBYSHELL);
						}
					}
					if (e->obj->ObjType == ObjType::BRICK)
					{
						if (e->nx != 0)
						{
							if (e->obj->y < y)
							{
								this->nx = -this->nx;
								vx = -vx;
							}
							else
								x += dx;
						}
						if (IsAttack && e->nx)
							e->obj->SubHealth();
					}
					if (e->nx && e->obj->ObjType != ItemType::MUSHROOM && e->obj->ObjType != ObjType::BLOCK && e->obj->ObjType != ObjType::KOOPAS && e->obj->ObjType != ObjType::REDKOOPAS && e->obj->ObjType != ObjType::GOOMBA && e->obj->ObjType != ObjType::QUESTIONBRICK && e->obj->ObjType != ObjType::BRICK)
					{

						this->nx = -this->nx;
						vx = -vx;

					}
					if (e->obj->ObjType == ObjType::BLOCK && e->nx)
						x += dx;
					if (ny != 0) vy = 0;
					if (e->ny < 0 && state == KOOPAS_STATE_DIEBYTAIL)
						vx = vy = 0;
					if (e->obj->ObjType != ItemType::MUSHROOM && e->obj->ObjType != ObjType::KOOPAS && e->obj->ObjType != ObjType::REDKOOPAS && e->obj->ObjType != ObjType::GOOMBA && e->obj->ObjType != ObjType::MARIO)
						y += min_ty * dy + ny * 0.5f;
					if (e->obj->ObjType != ObjType::BLOCK)
						x += min_tx * dx + nx * 0.5f;

				}
			}
		}
		else
		{
			x += dx;
		}
	}
	else
	{
		y += dy;
		if (y <= 288)
		{
			y = 288;
			vy = -vy;
		}
		else if (y >= 384)
		{
			y = 384;
			vy = -vy;
		}
	}
}
