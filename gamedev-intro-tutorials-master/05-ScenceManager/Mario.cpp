#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include"Ground.h"
#include "Goomba.h"
#include "Portal.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	ObjType = 1;
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	CounterSpeed = 0;
	IsFalling = IsFlying = IsRunning = IsSitting = IsRollBack = IsOnPlatForm = false;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	DebugOut(L"VX%f\n", vx);
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	// Simple fall down
	vy += MARIO_GRAVITY * dt;
	if (TPlusStack == 0)
	{
		TPlusStack = GetTickCount64();
	}
	if (IsRunning)
	{
		IncreaseStack();
	}
	else
	{
		if (level != MARIO_LEVEL_RACOON)
		{
			if (IsOnPlatForm)
				DecreaseStack();
		}
		else
		{
			if (!IsFlying)
				DecreaseStack();
		}
	}
	if (IsFlying && TCanFly == 0)
	{
		TCanFly = GetTickCount64();
	}
	if (GetTickCount64() - TCanFly >= 3000)
	{
		IsFlying = false;
		TCanFly = 0;
	}
	if (IsRollBack == true && TRollBack == 0)
	{
		TRollBack = GetTickCount64();
	}
	if (GetTickCount64() - TRollBack >= 100)
	{
		IsRollBack = false;
		TRollBack = 0;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	//DebugOut(L"Stack %d\n", CounterSpeed);
	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
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
			if (e->ny < 0)
			{
				IsOnPlatForm = true;
			}
			if (e->obj->ObjType == ObjType::GOOMBA) // if e->obj is Goomba 
			{
				//CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (e->obj->GetState() != GOOMBA_STATE_DIE)
					{
						e->obj->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (e->obj->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			if (e->obj->ObjType == ObjType::BLOCK)
			{
				x += dx;
			}
			else
			{
				if (nx != 0) vx = 0;
				x += min_tx * dx + nx * 0.5f;
			}
			y += min_ty * dy + ny * 0.5f;
			if (ny != 0) vy = 0;
		}
	}
	DebugOut(L"VelocityX%f\n", vx);
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
void CMario::Render()
{
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
		if (level == MARIO_LEVEL_BIG)
		{
			if (IsSitting)
			{
				if (nx >= 0)
					ani = MARIO_ANI_SITTINGRIGHT;
				else
					ani = MARIO_ANI_SITTINGLEFT;
			}
			else
			{
				if (IsOnPlatForm)
				{
					if (vx == 0)
					{
						if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
						else ani = MARIO_ANI_BIG_IDLE_LEFT;
					}
					else if (vx > 0)
					{
						if (IsRollBack)
						{
							ani = MARIO_RB_WALKINGLEFT;
						}
						else
						{
							ani = MARIO_ANI_BIG_WALKING_RIGHT;
						}
					}
					else if (vx < 0)
					{
						if (IsRollBack)
						{
							ani = MARIO_RB_WALKINGRIGHT;
						}
						else
						{
							ani = MARIO_ANI_BIG_WALKING_LEFT;
						}
					}
				}
				else
				{
					if (nx < 0)
					{
						if (CounterSpeed == 7)
							ani = MARIO_ANI_FLYJUMPLEFT;
						else if (vy < 0)
							ani = MARIO_ANI_JUMPINGLEFT;
						else
							ani = MARIO_ANI_FALLINGLEFT;
					}
					else
					{
						if (CounterSpeed == 7)
							ani = MARIO_ANI_FLYJUMPRIGHT;
						else if (vy < 0)
							ani = MARIO_ANI_JUMPINGRIGHT;
						else
							ani = MARIO_ANI_FALLINGRIGHT;
					}
				}
			}
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (IsOnPlatForm)
			{
				if (vx == 0)
				{
					
					if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					else ani = MARIO_ANI_SMALL_IDLE_LEFT;
				}
				else if (vx > 0)
				{
					if (IsRollBack)
						ani = MARIO_ANI_SMALL_ROLLBACKLEFT;
					else
						ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				}
				else
				{
					if (IsRollBack)
						ani = MARIO_ANI_SMALL_ROLLBACKRIGHT;
					else
						ani = MARIO_ANI_SMALL_WALKING_LEFT;
				}
			}
			else
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALLJUMPRIGHT;
				else
					ani = MARIO_ANI_SMALLJUMPLEFT;
			}
		}
	int alpha = 255;
	if (untouchable) alpha = 128;
	//DebugOut(L"\n%d",ani);
	if (IsRunning && IsOnPlatForm)
	{	
		if (CounterSpeed == 7 && abs(vx) == MARIO_MAX_SPEED * 2)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (vx > 0)
					ani = MARIO_ANI_FASTESTRUNRIGHT;
				else
					ani = MARIO_ANI_FASTESTRUNLEFT;
				animation_set->at(ani)->Render(x, y);
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (vx > 0)
					ani = MARIO_ANI_SMALL_FASTESTRUNRIGHT;
				else
					ani = MARIO_ANI_SMALL_FASTESTRUNLEFT;
				animation_set->at(ani)->Render(x, y);
			}
		}
		else 
			{
				animation_set->at(ani)->RenderFTime(x, y, 20);
			}
	}
	else
		animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		if (IsSitting)
			y = y - 9;
		IsSitting = false;
		if (vx < 0)
		{
			vx -= vx;
			IsRollBack = true;
		}
		else
		{
			if (!IsRunning)
			{
				if (vx <= MARIO_MAX_SPEED)
				{
					vx += MARIO_ACCLERATION;
					if (vx >= MARIO_MAX_SPEED)
					{
						vx = MARIO_MAX_SPEED;
					}
				}
				else
					vx = MARIO_MAX_SPEED;
			}
			else if (vx <= MARIO_MAX_SPEED * 2)
			{
				vx += MARIO_ACCLERATION;
				if (vx >= MARIO_MAX_SPEED * 2)
				{
					vx = MARIO_MAX_SPEED * 2;
				}
			}
		}
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (IsSitting)
			y = y - 9;
		IsSitting = false;
		if (vx > 0)
		{
			vx -= vx;
			IsRollBack = true;
		}
		else
		{
			if (!IsRunning)
			{
				if (vx >= -MARIO_MAX_SPEED)
				{
					vx -= MARIO_ACCLERATION;
					if (vx <= -MARIO_MAX_SPEED)
					{
						vx = -MARIO_MAX_SPEED;
					}
				}
				else
					vx = -MARIO_MAX_SPEED;
			}
			else if (vx >= -MARIO_MAX_SPEED * 2)
			{
				vx -= MARIO_ACCLERATION;
				if (vx <= -MARIO_MAX_SPEED * 2)
				{
					vx = -MARIO_MAX_SPEED * 2;
				}
			}
		}
		nx = -1;
		break;
	case MARIO_STATE_JUMP:

			if (IsOnPlatForm)
			{
				vy = -0.5;
				IsOnPlatForm = false;
			}
		break;
	case MARIO_STATE_IDLE:
		vx -= min(abs(vx), MARIO_FRICTION) * nx;
		if (IsSitting)
			y = y - 9;
		IsSitting = false;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_SIT:
		if (level != MARIO_LEVEL_SMALL)
		{
			if (!IsSitting)
				y = y + 9;
			IsSitting = true;
			vx -= min(abs(vx), MARIO_FRICTION) * nx;
		}
		break;
	case MARIO_STATE_RUN:
		if (vx != 0)
			IsRunning = true;
		break;
	case MARIO_STATE_FLY:
		if (CounterSpeed == 7 && level == MARIO_LEVEL_RACOON)
		{
			vy = -MARIO_FLY_SPEED;
			IsFlying = true;
		}
		break;
	case MARIO_STATE_FALLING:
		if (vy > 0 && !IsOnPlatForm && !IsFlying)
		{
			vy = 0.02;
		}
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (level == MARIO_LEVEL_BIG)
	{
		if (IsSitting)
		{
			bottom = y + 18;
			right = x + MARIO_BIG_BBOX_WIDTH;
		}
		else
		{
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
			right = x + MARIO_BIG_BBOX_WIDTH;
		}
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

