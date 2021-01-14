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
#include "RedKoopas.h"
#include "Button.h"
#include "Camera.h"
#include "BigCoin.h"

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
	IsHoldingKoopas = false;
	KP = new CKoopas();
	score = new Score();
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	// Simple fall down
	for (int i = 0; i < effects.size(); i++)
	{
		effects[i]->Update(dt);
	}
	if (!StopUpdate)
	{
		vy += GRAVITY * dt;
		if (level == MARIO_LEVEL_RACOON)
		{
			tail->AdaptMarioPos(x, y, nx);
			tail->Update(dt, coObjects);
		}
		if (IsAttack && level == MARIO_LEVEL_FIRE)
		{
			if (AmountofFirebullet > 0)
			{
				FireBullet* fb = new FireBullet(0, 0);
				fb->SetSpeed(0.2 * nx, 0);
				fb->SetPosition(x + 5, y);
				fb->FireMario = true;
				firebullet.push_back(fb);
				AmountofFirebullet--;
			}
			IsAttack = false;
		}
		for (int i = 0; i < firebullet.size(); i++)
			firebullet[i]->Update(dt, coObjects);
		if (y <= 0)
		{
			y = 0;
		}
		if (tail->IsActivated)
		{
			tail->AdoptAtkpos(TAttack);
		}
		if (IsAttack && GetTickCount64() - TAttack >= MARIO_RACOON_ATKTIME)
		{
			//animation_set->at(ani)->currentFrame = 0;
			//DebugOut(L"ATTACK\n");
			tail->IsActivated = false;
			IsAttack = false;
			tail->Attacking = false;
		}
		if (nx * vx >= 0 && IsRollBack)
		{
			IsEndRollBack = true;
			tail->StopRender = false;
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
		if (IsFlying)
		{
			if (TCanFly == 0)
			{
				TCanFly = GetTickCount64();
			}
			if (!IsOnPlatForm)
				tail->StopRender = true;
			else
				tail->StopRender = false;

		}
		if (GetTickCount64() - TCanFly >= MARIO_CANFLY_TIME)
		{
			IsFlying = false;
			tail->StopRender = false;
			TCanFly = 0;
		}
		if (IsRollBack == true )
		{
			tail->StopRender = true;
			if(TRollBack == 0)
			TRollBack = GetTickCount64();
		}
		if (GetTickCount64() - TRollBack >= 200 && IsEndRollBack)
		{
			IsRollBack = false;
			TRollBack = 0;
		}
		
		KP->ChkIsHolding(IsHoldingKoopas);
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		int aniID = 0;
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
			if (vy > 0.1)
			{
				IsOnPlatForm = false;
			}
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
				if (e->obj->ObjType != ObjType::BLOCK && e->obj->ObjType != ObjType::KOOPAS && e->obj->ObjType != ObjType::PORTAL&&e->obj->ObjType != ItemType::BIGCOIN)
				{
					if (e->nx != 0) {
						vx = 0; DecreaseStack();
					}
				}
				if (e->ny < 0)
				{
					if (e->obj->ObjType == ObjType::BUTTON)
					{
						e->obj->SetState(BUTTON_STATE_ISPRESSED);
					}
					if (e->obj->ObjType == ObjType::WARPPIPE)
					{
						WP = dynamic_cast<WarpPipe*>(e->obj);
						WP->SetState(WARPIPE_STATE_HAVE_MARIOON);
					}
					else
						WP->SetState(WARPIPE_STATE_NOTHAVE_MARIOON);
					vy = 0;
					IsOnPlatForm = true;
				}
				else if (e->ny > 0)
				{
					if(e->obj->ObjType!=ItemType::BIGCOIN)
						vy = 0;
					IsOnPlatForm = false;
				}
				if (e->obj->ObjType == ItemType::MUSHROOM)
				{
					e->obj->SubHealth();
					Health++;
					level = MARIO_LEVEL_BIG;
					y -= MARIO_SET_LEVEL_Y;
				}
				if (e->obj->ObjType == ItemType::BIGCOIN)
				{
					score->CalcScoreEarn();
					e->obj->SetState(BIGCOIN_STATE_COLLECTED);
					
					if (score->ScoreEarn == 100)
						aniID = 0;
					if (score->ScoreEarn == 200)
						aniID = 1;
					if (score->ScoreEarn == 400)
						aniID = 2;
					if (score->ScoreEarn == 800)
						aniID = 3;
					if (score->ScoreEarn == 1000)
						aniID = 4;
					Effect* Ef = new Effect(e->obj->x, e->obj->y, 40, aniID);
					Ef->SetState(40);
					effects.push_back(Ef);
					e->obj->SubHealth();
					x += dx;
					y += dy;
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
				else if (e->obj->ObjType == ObjType::QUESTIONBRICK)
				{
					if (e->ny > 0)
					{
						e->obj->SetState(BRICK_STATE_COLISSION);
						score->CalcScoreEarn();
					}
					else
						y += min_ty * dy + ny * 0.4f;
				}
				else if (e->obj->ObjType == ObjType::FIREPIRANHAPLANT)
				{
					Calclevel();
					if (e->ny != 0) vy = 0;
					if (e->nx != 0) vx = 0;
				}
				if (e->obj->ObjType == ObjType::GROUND)
				{
					if (e->ny < 0)
						y += min_ty * dy + ny * 0.4f;
					if (e->ny) vy = 0;
				}
				if (e->obj->ObjType == ObjType::LSPORTAL)
				{
					CPortal* portal = dynamic_cast<CPortal*>(e->obj);
					CGame::GetInstance()->SwitchScene(1);
				}
				if (e->obj->ObjType == ObjType::BLOCK)
				{
					x += dx;
					if (e->ny < 0)
						y += min_ty * dy + ny * 0.5f;
					if (e->ny) vy = 0;
				}
				if (e->obj->ObjType == ObjType::BRICK)
				{
					if (e->ny < 0)
						y += min_ty * dy + ny * 0.2f;
					//if (e->ny) vy = 0;
				}
				if (e->obj->ObjType == ObjType::PORTAL)
				{
					x += dx;
					//y += dy;
				}
				if (e->obj->ObjType == ObjType::KOOPAS || e->obj->ObjType == ObjType::REDKOOPAS && IsHoldingKoopas == false)
				{
					//Mario jump on the koopas
					if (e->ny < 0)
					{
						switch (e->obj->state)
						{
						case(KOOPAS_STATE_WALKING):
							e->obj->SetState(KOOPAS_STATE_HIDDEN);
							break;
						case(KOOPAS_STATE_HIDDEN):
							e->obj->nx = this->nx;
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
						if (e->obj->state != KOOPAS_STATE_HIDDEN)
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
								e->obj->nx = this->nx;
								e->obj->SetState(KOOPAS_STATE_HIDDEN_MOVE);
							}
						}
					}
					if (e->obj->ObjType == ObjType::KOOPAS) {
						KP = dynamic_cast<CKoopas*>(e->obj);
						KP->ChkIsHolding(IsHoldingKoopas);
					}
					else if (e->obj->ObjType == ObjType::REDKOOPAS) {
						KP = dynamic_cast<RedKoopas*>(e->obj);
						KP->ChkIsHolding(IsHoldingKoopas);
					}
				}
			}

		}
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->ObjType == ObjType::KOOPAS && coObjects->at(i)->state != KOOPAS_STATE_HIDDEN)
			{
				if (CheckAABB(coObjects->at(i))) {
					Calclevel();
				}
			}
			else if (coObjects->at(i)->ObjType == ItemType::LEAF)
			{
				if (CheckAABB(coObjects->at(i)))
				{
					coObjects->at(i)->SubHealth();
					level = MARIO_LEVEL_RACOON;
					y -= MARIO_SET_LEVEL_Y;
				}
			}
			if (coObjects->at(i)->ObjType == ObjType::PORTAL)
			{
				if (CheckAABB(coObjects->at(i)))
				{
					if (GoHiddenMap)
					{
						SetState(MARIO_STATE_GO_HIDDENMAP);
					}
					else if (GoOutHiddenMap)
					{
						SetState(MARIO_STATE_GO_OUT_HIDDENMAP);
					}
				}
			}
			if (coObjects->at(i)->ObjType == ItemType::BIGCOIN)
			{
				if (CheckAABB(coObjects->at(i)))
				{
					score->CalcScoreEarn();
					coObjects->at(i)->SetState(BIGCOIN_STATE_COLLECTED);
					coObjects->at(i)->SubHealth();
				}
			}
		}
		tail->AdaptMarioPos(x, y, nx);
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		/*DebugOut(L"currentLEVEL%d\n", level);*/
	}
	else
	{
		y += dy;
		if (y - StartYgoHiddenMap >= MARIO_RACOON_BBOX_HEIGHT)
		{
			Camera::GetInstance()->cam_y = y = HIDDEN_MAP_Y;
			x = HIDDEN_MAP_X;
			StopUpdate = false;
		}
		if (StartYgoHiddenMap - y >= 32)
		{
			Camera::GetInstance()->cam_y = 240;
			y = GO_OUT_HIDDEN_MAP_Y;
			x = GO_OUT_HIDDEN_MAP_X;
			StopUpdate = false;
		}
		tail->AdaptMarioPos(x, y, nx);
		if (!IsOnPlatForm && vy > 0 && !IsSitting && !IsFlying)
		{
			tail->SetState(TAIL_STATE_FALLING);
		}
	}
	if (IsHoldingKoopas)
	{
		KP->AdaptPos(x, y, nx);
	}
	if (!IsOnPlatForm && vy > 0 && !IsSitting && !IsFlying)
	{
		tail->SetState(TAIL_STATE_FALLING);
	}
	if (IsSitting)
		tail->StopRender = true;
	if (CounterSpeed == 7 && abs(vx) == MARIO_MAX_SPEED * 2)
	{
		tail->StopRender = true;
	}
}
void CMario::Render()
{
	int alpha = 255;
	if (untouchable) alpha = 128;
	if (!IsAttack && level==MARIO_LEVEL_RACOON)
	{
		animation_set->at(MARIO_ANI_RACOON_ATTACKRIGHT)->Reset();
		animation_set->at(MARIO_ANI_RACOON_ATTACKLEFT)->Reset();
		tail->animation_set->at(8)->Reset();
		tail->animation_set->at(9)->Reset();
	}
	if (level == MARIO_LEVEL_RACOON)
		tail->Render();
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
						if (IsHoldingKoopas)
						{
							if (nx > 0) ani = MARIO_ANI_BIG_IDLE_HOLDKPRIGHT;
							else ani = MARIO_ANI_BIG_IDLE_HOLDKPLEFT;
						}
						else
							if (nx > 0)ani = MARIO_ANI_BIG_IDLE_RIGHT;
							else ani = MARIO_ANI_BIG_IDLE_LEFT;
					}
					else if (nx > 0)
					{
						if (!IsHoldingKoopas)
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
						else
							ani = MARIO_ANI_BIG_WALKING_HOLDKP_RIGHT;
					}
					else if (nx < 0)
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
					{
						ani = MARIO_ANI_RACOON_ATTACKRIGHT;
					}
					else
					{
						ani = MARIO_ANI_RACOON_ATTACKLEFT;
					}
				}
				else
				{
					if (IsOnPlatForm)
					{
						if (vx == 0)
						{
							if (nx > 0) {
								ani = MARIO_ANI_RACOON_IDLE_RIGHT;
							}
							else {
								ani = MARIO_ANI_RACOON_IDLE_LEFT;
							}
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
							if (CounterSpeed == 7 && IsFlying)
							{
								ani = MARIO_ANI_RACOON_FLYLEFT;
							}
							else if (vy < 0)
							{
								ani = MARIO_ANI_RACOON_JUMPINGLEFT;
							}
							else if (IsSlowFalling)
							{
								ani = MARIO_ANI_RACOON_SLOWFALLLEFT;
							}
							else
							{
								tail->StopRender = false;
								ani = MARIO_ANI_RACOON_FALLINGLEFT;
							}
						}
						else
						{
							if (CounterSpeed == 7 && IsFlying)
							{
								ani = MARIO_ANI_RACOON_FLYRIGHT;
							}
							else if (vy < 0)
							{
								ani = MARIO_ANI_RACOON_JUMPINGRIGHT;
							}
							else if (IsSlowFalling)
							{
								ani = MARIO_ANI_RACOON_SLOWFALLRIGHT;
							}
							else
							{
								ani = MARIO_ANI_RACOON_FALLINGRIGHT;							
							}
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
		if (IsRunning && IsOnPlatForm && !IsAttack && !IsHoldingKoopas)
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
				{
					ani = MARIO_ANI_RACOON_FASTESTRUNRIGHT;
				}
				else
				{
					ani = MARIO_ANI_RACOON_FASTESTRUNLEFT;
				}
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
		if (IsHoldingKoopas)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (vx != 0)
				{
					if (nx > 0)
						animation_set->at(MARIO_ANI_BIG_WALKING_HOLDKP_RIGHT)->RenderHDKP(x+MARIO_BIG_BBOX_WIDTH, y);
					else
						animation_set->at(MARIO_ANI_BIG_WALKING_HOLDKP_LEFT)->Render(x, y);
				}
				else
					if (nx > 0)
						animation_set->at(MARIO_ANI_BIG_IDLE_HOLDKPRIGHT)->Render(x, y);
					else
						animation_set->at(MARIO_ANI_BIG_IDLE_HOLDKPLEFT)->Render(x, y);
			}
		}
	else if(ani == MARIO_ANI_RACOON_ATTACKRIGHT)
	{
		animation_set->at(ani)->Render(x-5, y);
	}
	else if (ani == MARIO_ANI_RACOON_ATTACKLEFT)
	{
		animation_set->at(ani)->RenderATK(x, y);
	}
	else
		{
			animation_set->at(ani)->Render(x , y);
		}
	for (int i = 0; i < firebullet.size(); i++)
	{
		firebullet.at(i)->Render();
	}
	//RenderBoundingBox();
	for (int i = 0; i < effects.size(); i++)
		effects[i]->Render();
	
	//DebugOut(L"vy%f\n", vy);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		if (!IsAttack)
		{
			if (IsSitting)
				y = y - 9;
			IsSitting = false;
			if (vx < 0)
			{
				vx += MARIO_DECLERATION;
				IsRollBack = true;
				tail->StopRender = true;
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
				tail->SetState(TAIL_STATE_WALKING);
			}
			nx = 1;
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (!IsAttack)
		{
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

				tail->SetState(TAIL_STATE_WALKING);
			}
			nx = -1;
		}
		break;
	case MARIO_STATE_JUMP:

			if (IsOnPlatForm)
			{
				vy = -MARIO_JUMP_SPEED_Y;
				IsOnPlatForm = false;
				tail->SetState(TAIL_STATE_JUMPING);
			}
		break;
	case MARIO_STATE_IDLE:
		if (IsOnPlatForm)
		{
			if (IsRollBack)
				vx += min(abs(vx), MARIO_FRICTION) * nx;
			else
				vx -= min(abs(vx), MARIO_FRICTION) * nx;
			if (IsSitting)
				y = y - MARIO_SET_IDLE_Y;
			IsSitting = false;
			if (!IsRollBack && !IsSitting)
				tail->SetState(TAIL_STATE_IDLING);
		}
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
		{
			IsRunning = true;
			DebugOut(L"hELLO\n");
		}
		break;
	case MARIO_STATE_FLY:
		if ((CounterSpeed == 7 && level == MARIO_LEVEL_RACOON && abs(vx) == MARIO_MAX_SPEED*2)||IsFlying)
		{
			if(!IsOnPlatForm)
			{
				vy = -MARIO_FLY_SPEED;
			if (!IsFlying)
			{
				IsFlying = true;
			}
			TAttack = GetTickCount64();
			IsOnPlatForm = false;
			}
		}
		break;
	case MARIO_STATE_SLOWFALLING:
		if (vy > 0 && !IsOnPlatForm && !IsFlying)
		{
			vy = 0.005;
			IsSlowFalling = true;
			tail->SetState(TAIL_STATE_SLOWFALLING);
		}
		break;
	case MARIO_STATE_ATTACK:
		IsAttack = true;
		TAttack = GetTickCount64();		
		if (level == MARIO_LEVEL_RACOON)
		{
			tail->IsActivated = true;
			tail->SetState(TAIL_STATE_ATTACKING);
		}
		break;
	case MARIO_STATE_GO_HIDDENMAP:
		vy = 0.05;
		StopUpdate = true;
		StartYgoHiddenMap = y;
		break;
	case MARIO_STATE_GO_OUT_HIDDENMAP:
		vy = -0.05;
		StartYgoHiddenMap = y; 
		StopUpdate = true;
		tail->StopRender = true;
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

