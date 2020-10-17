#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include"Brick.h"
#include"Ground.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (!IsFalling)
		vy += MARIO_GRAVITY * dt;
	else
		vy = 0.02 ;
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
		if(!IsFlying)
			DecreaseStack();
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
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
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
			else if(dynamic_cast<Ground*>(e->obj))
			{
				// block 
				x += min_tx * dx + nx * 0.6f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
				y += min_ty * dy + ny * 0.6f;
			}			
			if (e->ny < 0)
			{
				IsOnPlatForm = true;
			}
			if (ny != 0) vy = 0;
			if (nx != 0) vx = 0;
		}
	}
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
						if (vy < 0)
							ani = MARIO_ANI_JUMPINGLEFT;
						else
							ani = MARIO_ANI_FALLINGLEFT;
					}
					else
					{
						if (vy < 0)
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
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				else ani = MARIO_ANI_SMALL_WALKING_LEFT;
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
	if (IsRunning)
	{
		if (CounterSpeed == 7&&abs(vx)== MARIO_MAX_SPEED * 2)
		{
			if (vx > 0)
				ani = MARIO_ANI_FASTESTRUNRIGHT;
			else
				ani = MARIO_ANI_FASTESTRUNLEFT;
		animations[ani]->Render(x, y);
		}		
		else
		{
			animations[ani]->RenderFTime(x, y, 20);
		}
	}
	else
		animations[ani]->Render(x, y, alpha);	
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
		if (IsOnPlatForm || vy < 0)
		{
			if (IsOnPlatForm)
			{
				CalStartJumpPosition();
				IsOnPlatForm = false;
			}
			if (StartJump - y < MARIO_MAX_HIGH_JUMP)
			{
				vy = -MARIO_JUMP_SPEED_Y;
			}
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
		if (CounterSpeed == 7 /*&& level == MARIO_LEVEL_RACOON*/)
		{
			vy = -MARIO_FLY_SPEED;
			IsFlying = true;
		}
		break;
	case MARIO_STATE_FALLING:
		if (vy > 0 && !IsOnPlatForm && !IsFlying)
			IsFalling = true;
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

