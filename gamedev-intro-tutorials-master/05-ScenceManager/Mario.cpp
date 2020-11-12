#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include"Ground.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"
#include"QuestionBrick.h"
#include "MushRoom.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	ObjType = ObjType::MARIO;
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	CounterSpeed = 0;
	IsMovingObject = true;
	IsEndRollBack = IsFlying = IsRunning = IsSitting = IsRollBack = IsOnPlatForm = false;
	AmountofFirebullet = 2;
	FireBullet* fb1 = new FireBullet(0, 0);
	FireBullet* fb2 = new FireBullet(0, 0);
	firebullet.push_back(fb1);
	firebullet.push_back(fb2);
	IsHoldingKoopas = false;
	KP = new CKoopas();
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	// Simple fall down
	vy += GRAVITY * dt;
	for (int i = 0; i < 2; i++)
	{
		if (firebullet[i]->GetHealth() == 0 && IsAttack)
		{
			AmountofFirebullet = i+1;
		}
	}
	if (IsAttack && level == MARIO_LEVEL_FIRE)
	{
		if (AmountofFirebullet > 0)
		{
			FireBullet* fb=new FireBullet(0,0);
			fb->SetSpeed(0.1 * nx, 0);
			fb->SetPosition(x + 5, y);
			fb->FireMario = true;
			firebullet[AmountofFirebullet - 1] = fb;
			AmountofFirebullet--;
		}
		IsAttack = false;
	}
	for (int i = 0; i < 2; i++)
		firebullet[i]->Update(dt, coObjects);

	if (IsAttack && GetTickCount64() - TAttack >= MARIO_RACOON_ATKTIME)
	{
		//animation_set->at(ani)->currentFrame = 0;
		//DebugOut(L"ATTACK\n");
		IsAttack = false;
	}
	if (nx * vx >= 0 && IsRollBack)
	{
		IsEndRollBack = true;
		vx = 0;
	}
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
	if (GetTickCount64() - TCanFly >= MARIO_CANFLY_TIME)
	{
		IsFlying = false;
		TCanFly = 0;
	}
	if (IsRollBack == true && TRollBack == 0)
	{
		TRollBack = GetTickCount64();
	}
	if (GetTickCount64() - TRollBack >= 200 && IsEndRollBack)
	{
		IsRollBack = false;
		TRollBack = 0;
	}
	if (IsHoldingKoopas)
	{
		AdoptPosHolding();
	}
	KP->ChkIsHolding(IsHoldingKoopas);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
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
		x += min_tx * dx + nx * 0.5f;
		
		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->ObjType != ObjType::BLOCK)
			{
				if (e->nx != 0) vx = 0;
			}
			if (e->ny < 0)
			{
				IsOnPlatForm = true;
			}
			else
			{
				IsOnPlatForm = false;
			}
			if (e->obj->ObjType == ItemType::MUSHROOM)
			{
				e->obj->SubHealth();
				if (level < MARIO_LEVEL_FIRE)
				{
					Health++;
					this->level++;
					y -= MARIO_SET_LEVEL_Y;
				}
			}
			if (e->obj->ObjType == ObjType::GOOMBA) // if e->obj is Goomba 
			{
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
							Calclevel();
						}
					}
				}
			}
			
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (e->obj->ObjType == ObjType::QUESTIONBRICK)
			{
				if (e->ny > 0)
				{
					e->obj->SetState(BRICK_STATE_COLISSION);
				}
			}
			else if (e->obj->ObjType == ObjType::FIREPIRANHAPLANT)
			{
				Calclevel();
				if (e->ny != 0) vy = 0;
				if (e->nx != 0) vx = 0;
			}
			if (e->obj->ObjType == ObjType::GROUND)
			{
				y += min_ty * dy + ny * 0.4f;
			}
			if (e->obj->ObjType == ObjType::BLOCK)
			{
				x += dx;
				y += min_ty * dy + ny * 0.4f;
			}
			
			if (e->ny) vy = 0;
			if (e->obj->ObjType == ObjType::KOOPAS) 
			{
				e->obj->nx = this->nx;
				//Mario jump on the koopas
				if (e->ny < 0)
				{
					switch (e->obj->state)
					{
					case(KOOPAS_STATE_WALKING):
						e->obj->SetState(KOOPAS_STATE_HIDDEN);
						break;
					case(KOOPAS_STATE_HIDDEN):
						e->obj->SetState(KOOPAS_STATE_HIDDEN_MOVE);
						break;
					case(KOOPAS_STATE_HIDDEN_MOVE):
						e->obj->SetState(KOOPAS_STATE_HIDDEN);
						break;
					}
					vy = -MARIO_DIE_DEFLECT_SPEED;
					y += min_ty * dy + ny * 0.6f;
				}
				else if (e->nx != 0)
				{
					if (e->obj->state == KOOPAS_STATE_HIDDEN_MOVE)
					{
							Calclevel();
					}
					else
					{
						if (IsRunning)
						{
							IsHoldingKoopas = true;
						}	
						else if (e->obj->state != KOOPAS_STATE_HIDDEN_MOVE && e->obj->state == KOOPAS_STATE_HIDDEN)
						{
							e->obj->SetState(KOOPAS_STATE_HIDDEN_MOVE);
						}
					}
				}
				KP = dynamic_cast<CKoopas*>(e->obj);
				KP->ChkIsHolding(IsHoldingKoopas);
			}
		}
		
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	/*DebugOut(L"currentLEVEL%d\n", level);*/
	}
void CMario::Render()
{
	int alpha = 255;
	if (untouchable) alpha = 128;
	if (!IsAttack && (ani == MARIO_ANI_RACOON_ATTACKRIGHT|| ani == MARIO_ANI_RACOON_ATTACKLEFT))
		animation_set->at(ani)->Reset();
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
							ani = MARIO_RB_WALKINGRIGHT;
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
							ani = MARIO_RB_WALKINGLEFT;
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
						{
							 if (CounterSpeed == 7)
								ani = MARIO_ANI_FLYJUMPLEFT;
							 else
								ani = MARIO_ANI_JUMPINGLEFT;
						}
						else
							ani = MARIO_ANI_FALLINGLEFT;
					}
					else
					{
						if (vy < 0)
						{
							if (CounterSpeed == 7)
								ani = MARIO_ANI_FLYJUMPRIGHT;
							else
								ani = MARIO_ANI_JUMPINGRIGHT;
						}
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
						ani = MARIO_ANI_SMALL_ROLLBACKRIGHT;
					else
						ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				}
				else
				{
					if (IsRollBack)
						ani = MARIO_ANI_SMALL_ROLLBACKLEFT;
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
		else if (level == MARIO_LEVEL_RACOON)
		{
			if (IsSitting)
			{
				if (nx >= 0)
					ani = MARIO_ANI_RACOON_SITTINGRIGHT;
				else
					ani = MARIO_ANI_RACOON_SITTINGLEFT;
			}
			else
			{
				if (IsAttack)
				{
					if (nx > 0)
						ani = MARIO_ANI_RACOON_ATTACKRIGHT;
					else
						ani = MARIO_ANI_RACOON_ATTACKLEFT;
				}
				else
				{
					if (IsOnPlatForm)
					{
						if (vx == 0)
						{
							if (nx > 0) ani = MARIO_ANI_RACOON_IDLE_RIGHT;
							else ani = MARIO_ANI_RACOON_IDLE_LEFT;
						}
						else if (vx > 0)
						{
							if (IsRollBack)
							{
								ani = MARIO_ANI_RACOON_RBWALKINGRIGHT;
							}
							else
							{
								ani = MARIO_ANI_RACOON_WALKING_RIGHT;
							}
						}
						else if (vx < 0)
						{
							if (IsRollBack)
							{
								ani = MARIO_ANI_RACOON_RBWALKINGLEFT;
							}
							else
							{
								ani = MARIO_ANI_RACOON_WALKING_LEFT;
							}
						}
					}
					else
					{
						if (nx < 0)
						{
							if (CounterSpeed == 7)
								ani = MARIO_ANI_RACOON_FLYLEFT;
							else if (vy < 0)
								ani = MARIO_ANI_RACOON_JUMPINGLEFT;
							else if (IsSlowFalling)
								ani = MARIO_ANI_RACOON_SLOWFALLLEFT;
							else
								ani = MARIO_ANI_RACOON_FALLINGLEFT;
						}
						else
						{
							if (CounterSpeed == 7)
								ani = MARIO_ANI_RACOON_FLYRIGHT;
							else if (vy < 0)
								ani = MARIO_ANI_RACOON_JUMPINGRIGHT;
							else if (IsSlowFalling)
								ani = MARIO_ANI_RACOON_SLOWFALLRIGHT;
							else
								ani = MARIO_ANI_RACOON_FALLINGRIGHT;
						}
					}
				}
			}

		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (IsSitting)
			{
				if (nx >= 0)
					ani = MARIO_ANI_FIRE_SITTINGRIGHT;
				else
					ani = MARIO_ANI_FIRE_SITTINGLEFT;
			}
			else
			{
				if (IsOnPlatForm)
				{
					if (vx == 0)
					{
						if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
						else ani = MARIO_ANI_FIRE_IDLE_LEFT;
					}
					else if (vx > 0)
					{
						if (IsRollBack)
						{
							ani = MARIO_ANI_FIRE_RBWALKINGRIGHT;
						}
						else
						{
							ani = MARIO_ANI_FIRE_WALKING_RIGHT;
						}
					}
					else if (vx < 0)
					{
						if (IsRollBack)
						{
							ani = MARIO_ANI_FIRE_RBWALKINGLEFT;
						}
						else
						{
							ani = MARIO_ANI_FIRE_WALKING_LEFT;
						}
					}
				}
				else
				{
					if (nx < 0)
					{
						if (CounterSpeed == 7)
							ani = MARIO_ANI_FIRE_FLYJUMPLEFT;
						else if (vy < 0)
							ani = MARIO_ANI_FIRE_JUMPINGLEFT;
						else
							ani = MARIO_ANI_FIRE_FALLINGLEFT;
					}
					else
					{
						if (CounterSpeed == 7)
							ani = MARIO_ANI_FIRE_FLYJUMPRIGHT;
						else if (vy < 0)
							ani = MARIO_ANI_FIRE_JUMPINGRIGHT;
						else
							ani = MARIO_ANI_FIRE_FALLINGRIGHT;
					}
				}
			}
		}
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
			else if (level == MARIO_LEVEL_RACOON)
			{
				if (vx > 0)
					ani = MARIO_ANI_RACOON_FASTESTRUNRIGHT;
				else
					ani = MARIO_ANI_RACOON_FASTESTRUNLEFT;
				animation_set->at(ani)->Render(x, y);
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (vx > 0)
					ani = MARIO_ANI_FIRE_FASTESTRUNRIGHT;
				else
					ani = MARIO_ANI_FIRE_FASTESTRUNLEFT;
				animation_set->at(ani)->Render(x, y);
			}
		}
		else
		{
			animation_set->at(ani)->RenderFTime(x, y, 20);
		}
	}
	else if (ani == MARIO_ANI_RACOON_ATTACKLEFT)
		animation_set->at(ani)->RenderATK(x, y);
	else
		animation_set->at(ani)->Render(x, y);
	for (int i = 0; i < 2; i++)
		firebullet[i]->Render();
	//RenderBoundingBox();
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
			vx += MARIO_DECLERATION;
			IsRollBack = true;
			if (IsEndRollBack)
				IsEndRollBack = false;
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
			vx -= MARIO_DECLERATION;
			IsRollBack = true;
			if (IsEndRollBack)
				IsEndRollBack = false;
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
				vy = -MARIO_JUMP_SPEED_Y;
				IsOnPlatForm = false;
			}
		break;
	case MARIO_STATE_IDLE:
		if (IsRollBack)
			vx += min(abs(vx), MARIO_FRICTION) * nx;
		else
			vx -= min(abs(vx), MARIO_FRICTION) * nx;
		if (IsSitting)
			y = y - MARIO_SET_IDLE_Y;
		IsSitting = false;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_SIT:
		if (level != MARIO_LEVEL_SMALL)
		{
			if (!IsSitting)
				y = y + MARIO_SET_IDLE_Y;
			IsSitting = true;
			if (IsRollBack)
				vx += min(abs(vx), MARIO_FRICTION) * nx;
			else
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
			if (!IsFlying)
			{
				IsFlying = true;
			}
		}
		break;
	case MARIO_STATE_SLOWFALLING:
		if (vy > 0 && !IsOnPlatForm && !IsFlying)
		{
			vy = 0.01;
			IsSlowFalling = true;
		}
		break;
	case MARIO_STATE_ATTACK:
		IsAttack = true;
		TAttack = GetTickCount64();
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (level != MARIO_LEVEL_SMALL)
	{

		if (IsSitting)
		{
			bottom = y + MARIO_SET_SITTING_Y;
			right = x + MARIO_BIG_BBOX_WIDTH;
		}
		else if (level != MARIO_LEVEL_RACOON)
		{
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
			right = x + MARIO_BIG_BBOX_WIDTH;
		}
		else
		{
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
			right = x + MARIO_RACOON_BBOX_WIDTH;
		}
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::Calclevel()
{
	if (level == MARIO_LEVEL_RACOON || level == MARIO_LEVEL_FIRE)
		SetLevel(MARIO_LEVEL_BIG);
	else if (level == MARIO_LEVEL_BIG)
		SetLevel(MARIO_LEVEL_SMALL);
	else if (level == MARIO_LEVEL_SMALL)
		SetState(MARIO_STATE_DIE);
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_RACOON);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

