#include"Koopas.h"
CKoopas::CKoopas()
{
	ObjType = ObjType::KOOPAS;
	SetState(KOOPAS_STATE_WALKING);
	IsMovingObject = true;
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!IsHolding)
	{
		left = x;
		top = y;
		right = x + KOOPAS_BBOX_WIDTH;
		bottom = y + 25;
		if (IsHidden)
		{
			bottom = y + 16;
		}
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
		CGameObject::Update(dt, coObjects);
		//
		// TO-DO: make sure Goomba can interact with the world and to each of them too!
		// 
		if (IsHidden && !IsAttack && !IsReborning)
		{
			if (GetTickCount64() - TimeStartReborn >= 7000)
			{
				SetState(KOOPAS_STATE_REBORN);
			}
		}
		if (IsReborning)
		{
			if (GetTickCount64() - TimeReborn >= 3600)
			{
				IsReborning = false;
				y -= 9;
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
					if (e->nx)
					{
						DebugOut(L"Goomba\n");
						e->obj->SetState(GOOMBA_STATE_DIE);
					}
				}
				else if (e->nx && e->obj->ObjType != ItemType::MUSHROOM)
				{
					vx = -vx;
				}
				
				x += dx;
				if (ny != 0) vy = 0;
				y += min_ty * dy + ny * 0.4f;
			}
		}
		}
		else
		{
			/*if (state != KOOPAS_STATE_HIDDEN)
				SetState(KOOPAS_STATE_HIDDEN);*/
		}
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx < 0) ani = KOOPAS_ANI_WALKING_LEFT;
	if (IsHidden)
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
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		IsHidden = false;
		IsAttack = false;
		IsReborning = false;
		IsWalking = true;
		vx = nx*0.02;
		break;
	case KOOPAS_STATE_HIDDEN:
		if (!IsHidden)
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
		IsWalking = false;
		TimeReborn = GetTickCount64();
		break;
	}

}