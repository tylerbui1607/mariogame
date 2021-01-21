#include"Koopas.h"
CKoopas::CKoopas(int level)
{
	ObjType = ObjType::KOOPAS;

	SetState(KOOPAS_STATE_WALKING);
	IsMovingObject = true;
	Level = level;
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{

		left = x;
		top = y;
		right = x + KOOPAS_BBOX_WIDTH;
		bottom = y + 25;
		if (IsHidden || IsDead)
		{
			bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
		}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
		CGameObject::Update(dt, coObjects);
		if (!IsDieByShell)
		{
			if (IsHidden && !IsReborning)
			{
				if (GetTickCount64() - TimeStartReborn >= 7000)
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
						else if (e->nx && e->obj->ObjType != ItemType::MUSHROOM && e->obj->ObjType != ObjType::BLOCK && e->obj->ObjType != ObjType::KOOPAS && e->obj->ObjType != ObjType::REDKOOPAS && e->obj->ObjType != ObjType::GOOMBA)
						{
							this->nx *= -1;
							vx = -vx;
						}

						x += dx;
						if (ny != 0) vy = 0;
						if (e->ny < 0 && state == KOOPAS_STATE_DIEBYTAIL)
						{
							vx = vy = 0;
						}
						if (e->ny < 0 && Level == KOOPAS_LEVEL_PARAKOOPAS)
							vy = -0.2;
						if (e->obj->ObjType != ItemType::MUSHROOM && e->obj->ObjType != ObjType::KOOPAS && e->obj->ObjType != ObjType::REDKOOPAS && e->obj->ObjType != ObjType::GOOMBA && e->obj->ObjType != ObjType::MARIO)
							y += min_ty * dy + ny * 0.5f;
					}
				}
			}
		}
		else
		{
			vy += GRAVITY * dt;
			x += dx;
			y += dy;
		}
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (Level == KOOPAS_LEVEL_NORMAL)
	{
		if (IsDead) {
			ani = KOOPAS_ANI_HIDDEN_DIE;
		}
		else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
		else if (vx < 0) ani = KOOPAS_ANI_WALKING_LEFT;
		if (IsHidden && !IsDead)
		{
			if (vx == 0 || IsHolding == true)
				ani = KOOPAS_ANI_HIDDEN;
			else
				ani = KOOPAS_ANI_HIDDENMOVE;
		}
		if (IsReborning)
		{
			ani = 4;
		}
	}
	else {
		if (nx > 0)
			ani = KOOPAS_ANI_FLYRIGHT;
		else
			ani = KOOPAS_ANI_FLYLEFT;
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		if (!IsDead)
		{
			IsDead = true;
			y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
		}
		break;
	case KOOPAS_STATE_WALKING:
		if (Level == KOOPAS_LEVEL_NORMAL)
		{
			IsHidden = false;
			IsAttack = false;
			IsReborning = false;
			IsHolding = false;
			IsWalking = true;
		}
		vx = nx*0.02;
		break;
	case KOOPAS_STATE_HIDDEN:
		if (!IsHidden && !IsDead)
		{
			y += 9;
		}
		vx = 0;
		IsHidden = true;
		IsAttack = false;
		IsWalking = false;
		TimeStartReborn = GetTickCount64();
		break;
	case KOOPAS_STATE_HIDDEN_MOVE:
		IsAttack = true;
		IsWalking = false;
		vx = nx*0.1;
		break;
	case KOOPAS_STATE_REBORN:
		IsReborning = true;
		IsAttack = true;
		IsWalking = false;
		IsDead = false;
		TimeReborn = GetTickCount64();
		break;
	case KOOPAS_STATE_DIEBYTAIL:
		if(!IsDead)
			y += 9;
		IsDead = true;
		IsHidden = true;
		IsAttack = false;
		vy = -0.2;
		TimeStartReborn = GetTickCount64();
		break;
	case KOOPAS_STATE_DIEBYSHELL:
		if (!IsDead)
			y += 9;
		IsDead = true;
		IsHidden = true;
		IsAttack = false;
		IsDieByShell = true;
		vy = -0.2;
		break;
	}

}