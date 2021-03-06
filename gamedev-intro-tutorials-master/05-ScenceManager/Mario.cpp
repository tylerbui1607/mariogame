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
#include "FlyingWood.h"
#include "LastSceenItem.h"
#include "Hud.h"

CMario* CMario::__instance = NULL;
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
	StopUpdate = IsEndRollBack = IsFlying = IsRunning = IsSitting = IsRollBack = IsOnPlatForm = false;
	AmountofFirebullet = 2;
	IsHoldingKoopas = false;
	FireBullet* fb1 = new FireBullet(-50, 0);
	FireBullet* fb2 = new FireBullet(-50, 0);
	firebullet.push_back(fb1);
	firebullet.push_back(fb2);
	KP = new CKoopas();
	Money = 0;
	score = new Score();
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (x <= 0)
	{
		x = 0;
	}
	if (x + MARIO_BIG_BBOX_WIDTH > 2816 && !EndScene)
	x = 2816 - MARIO_BIG_BBOX_WIDTH;
	if (Camera::GetInstance()->AutoMove == 1)
	{
		if (x < Camera::GetInstance()->cam_x)
		{
			x = Camera::GetInstance()->cam_x;
		}
	}
	for (int i = 0; i < effects.size(); i++)
	{
		effects[i]->Update(dt);
	}
	if (!StopUpdate)
	{
		if (untouchable == 1 && GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
		{
			untouchable_start = 0;
			untouchable = 0;
		}
		vy += GRAVITY * dt;
		if (level == MARIO_LEVEL_RACOON)
		{
			tail->AdaptMarioPos(x, y, nx);
			tail->Update(dt, coObjects);
		}
		if (IsAttack && level == MARIO_LEVEL_FIRE)
		{
			for(int i=0;i<2;i++)
			{
				if (!firebullet[i]->Attack)
				{
					firebullet[i]->SetPosition(x + 5, y);
					firebullet[i]->SetSpeed(0.2 * nx, 0);
					firebullet[i]->FireMario = true;
					firebullet[i]->Attack = true;
					i = 2;
				}
			}
		}
		for (int i = 0; i < firebullet.size(); i++)
		{
			if (firebullet[i]->Attack)
			{
				firebullet[i]->Update(dt, coObjects);
			}
		}
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
			tail->IsActivated = false;
			IsAttack = false;
		}
		if (IsKickKoopas)
		{
			if (GetTickCount64() - Kick >= 200)
			{
				IsKickKoopas = false;
			}
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

		}
		if (GetTickCount64() - TCanFly >= MARIO_CANFLY_TIME)
		{
			IsFlying = false;
			TCanFly = 0;
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
		if (EndScene && IsOnPlatForm)
		{
			nx = 1;
			SetSpeed(0.15, 0);
		}
		if (EndScene && GetTickCount64() - Endscene >= 1500)
		{
			Hud::GetInstance()->CourseClear = true;
			if (GetTickCount64() - Endscene >= 3500)
			{
				NextSceneID = 1;
				IsSwitchScene = true;
			}
		}
		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
			if (vy > 0.1 && !IsOnWood)
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
				if (e->obj->ObjType != ObjType::BLOCK && e->obj->ObjType != ObjType::KOOPAS && e->obj->ObjType != ObjType::PORTAL && e->obj->ObjType != ObjType::REDKOOPAS && e->obj->ObjType != ObjType::BRICK)
				{
					if (e->nx != 0) {
							vx = 0;
							if (!IsFlying)
								DecreaseStack();
					}
				}
				if (e->ny < 0)
				{
					if (e->obj->ObjType == ObjType::MOVINGWOOD)
					{
						FlyingWood* FW = dynamic_cast<FlyingWood*>(e->obj);
						FW->IsMarioOn = true;
						IsOnWood = true;
					}
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
					if (e->obj->ObjType != ObjType::MOVINGWOOD)
						vy = 0;
					else
						vy = e->obj->vy-0.01;
					IsOnPlatForm = true;
				}
				else if (e->ny > 0)
				{
					if (e->obj->ObjType != ItemType::BIGCOIN)
					{
						if (e->obj->ObjType == ObjType::BRICK && e->obj->vy == 0)
							vy = 0;
						else if (e->obj->ObjType != ObjType::BRICK)
							vy = 0;
					}
					IsOnPlatForm = false;
				}
				if (e->obj->ObjType == ItemType::MUSHROOM)
				{
					e->obj->SubHealth();
					Health++;
					level = MARIO_LEVEL_BIG;
					y -= MARIO_SET_LEVEL_Y;
				}
				if (e->obj->ObjType == ObjType::MOVINGWOOD)
				{
					if (e->nx != 0)
					{
						x += min_tx * dx + nx * 0.5f;
						x = e->obj->x - 16;
					}
				}
				if (e->obj->ObjType == ItemType::LASTITEM)
				{
					LastSceenItem* LI = dynamic_cast<LastSceenItem*>(e->obj);
					LI->SetState(STATE_ACTIVE);
					LI->CalcLastItem();
					LastItem = LI->Item;
					Hud::GetInstance()->Item.push_back(LastItem);
					this->SetState(MARIO_STATE_FINISH);
				}
				if (e->obj->ObjType == ObjType::GOOMBA) // if e->obj is Goomba 
				{
					if (e->ny < 0)
					{
						if (e->obj->GetState() != GOOMBA_STATE_DIE)
						{
							CGoomba* gb = dynamic_cast<CGoomba*>(e->obj);
							int ani=0;
							if (gb->Level == GOOMBA_LEVEL_NORMAL)
								e->obj->SetState(GOOMBA_STATE_DIE);
							else
							{
								gb->Level--;
								gb->SetState(GOOMBA_STATE_WALKING);
							}
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							score->CalcScoreEarn();
							switch (score->ScoreEarn) 
							{
							case 100:
								ani = 0;
								break;
							case 200:
								ani = 1;
								break;
							case 400:
								ani = 2;
								break;
							case 800:
								ani = 3;
								break;
							case 1000:
								ani = 4;
								break;
							}
							Effect* ef = new Effect(e->obj->x, e->obj->y, 40, ani);
							ef->SetState(40);
							effects.push_back(ef);
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
									Calclevel();
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
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
					NextSceneID = portal->GetSceneId();
					IsSwitchScene = true;
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
					if (e->nx != 0)
					{	
						if (e->obj->y < y + MARIO_BIG_BBOX_HEIGHT)
						{
							vx = 0;
						}
						else
						{
							x += dx;
						}
					}
					if (e->ny > 0)
					{
						CBrick* brick = dynamic_cast<CBrick*>(e->obj);
						if (brick->ItemType != ObjType::BUTTON)
						{
							brick->SetState(BRICK_STATE_BROKEN);
						}
						else
							brick->SetState(BRICK_STATE_COLLISION);
					}
					else if (e->ny < 0)
						y += min_ty * dy + ny * 0.15f;
				}
				if (e->obj->ObjType == ObjType::PORTAL)
				{
					x += dx;
					//y += dy;
				}
				if (e->obj->ObjType == ObjType::KOOPAS || e->obj->ObjType == ObjType::REDKOOPAS && IsHoldingKoopas == false)
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
					//Mario jump on the koopas
					if (e->ny < 0)
					{
						
						if (koopas->IsPara)
						{
							koopas->IsPara = false;
						}
						else if (koopas->Level == KOOPAS_LEVEL_NORMAL)
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
							case(KOOPAS_STATE_DIE):
								e->obj->nx = this->nx;
								e->obj->SetState(KOOPAS_STATE_HIDDEN_MOVE); break;
							}
						}
						else
							koopas->Level--;
						vy = -MARIO_DIE_DEFLECT_SPEED;
						y += min_ty * dy + ny * 0.5f;
					}
					else if (e->nx != 0)
					{
						if (e->obj->state != KOOPAS_STATE_HIDDEN && e->obj->state != KOOPAS_STATE_DIEBYTAIL && e->obj->state != KOOPAS_STATE_DIE && e->obj->state != KOOPAS_STATE_DIEBYTAIL && !IsHoldingKoopas)
						{
						
							if (untouchable == 0)
							{
									if (level > MARIO_LEVEL_SMALL)
									{
										Calclevel();
										StartUntouchable();
									}
									else
										SetState(MARIO_STATE_DIE);
							}
						}
						else
						{
							if (IsRunning)
							{
								IsHoldingKoopas = true;
								KP = dynamic_cast<CKoopas*>(e->obj);
								if (!KP->IsReborning)
									KP->ChkIsHolding(IsHoldingKoopas);
							}
							else if (e->obj->state != KOOPAS_STATE_HIDDEN_MOVE && !IsHoldingKoopas)
							{
								if (!IsKickKoopas)
								{
									IsKickKoopas = true;
									Kick = GetTickCount64();
								}
								if(e->obj->state== KOOPAS_STATE_HIDDEN|| e->obj->state == KOOPAS_STATE_DIEBYTAIL || e->obj->state == KOOPAS_STATE_DIE)
								e->obj->nx = this->nx;
								e->obj->SetState(KOOPAS_STATE_HIDDEN_MOVE);
							}
						}
					}
				}
			}

		}
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->ObjType == ObjType::REDKOOPAS || coObjects->at(i)->ObjType == ObjType::KOOPAS)
			{
				CKoopas* kp = dynamic_cast<CKoopas*>(coObjects->at(i));
				if (CheckAABB(coObjects->at(i))) {
					if (!KP->IsHolding || KP->IsAttack)
					{
						if (untouchable == 0)
						{
							Calclevel();
							StartUntouchable();
						}
					}
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
					Money++;
					coObjects->at(i)->SetState(BIGCOIN_STATE_COLLECTED);
					coObjects->at(i)->SubHealth();
				}
			}
			if (coObjects->at(i)->ObjType == ObjType::BULLET)
			{
				if (CheckAABB(coObjects->at(i)))
				{
					Calclevel();
				}
			}
		}
		tail->AdaptMarioPos(x, y, nx);
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	else
	{
		y += dy;
		if (y - StartYgoHiddenMap >= MARIO_RACOON_BBOX_HEIGHT)
		{
			if (Camera::GetInstance()->AutoMove != 1)
			{
				Camera::GetInstance()->cam_y = y = HIDDEN_MAP_Y;
				x = HIDDEN_MAP_X;
			}
			else
			{
				Camera::GetInstance()->cam_y = 240;
				Camera::GetInstance()->AutoMove++;
				Camera::GetInstance()->cam_x = 2096;
				y = 352;
				x = 2192;
			}
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
	}
	if (KP->IsHolding)
	{
		if (level != MARIO_LEVEL_SMALL)
			KP->AdaptPos(x, y, nx);
		else
			KP->AdaptPosSmall(x, y, nx);
	}
	if (x + MARIO_BIG_BBOX_WIDTH >= MAP_MAX_WIDTH)
	{
		x = MAP_MAX_WIDTH - MARIO_BIG_BBOX_WIDTH - 1;
		vx = 0;
	}
}
void CMario::Render()
{
	int alpha = 255;
	if (untouchable) { alpha = 128;}
	if (!IsAttack && level==MARIO_LEVEL_RACOON)
	{
		animation_set->at(MARIO_ANI_RACOON_ATTACKRIGHT)->Reset();
		animation_set->at(MARIO_ANI_RACOON_ATTACKLEFT)->Reset();
	}
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
									if (CounterSpeed == MAX_STACK_POWER )
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
									if (CounterSpeed == MAX_STACK_POWER )
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
							if (CounterSpeed == MAX_STACK_POWER  && IsFlying)
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
								ani = MARIO_ANI_RACOON_FALLINGLEFT;
							}
						}
						else
						{
							if (CounterSpeed == MAX_STACK_POWER  && IsFlying)
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
			if (IsAttack)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_FIRE_ATTACKRIGHT;
				}
				else
				{
					ani = MARIO_ANI_FIRE_ATTACKLEFT;
				}
			}
			else if (IsSitting)
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
						if (CounterSpeed == MAX_STACK_POWER )
							ani = MARIO_ANI_FIRE_FLYJUMPLEFT;
						else if (vy < 0)
							ani = MARIO_ANI_FIRE_JUMPINGLEFT;
						else
							ani = MARIO_ANI_FIRE_FALLINGLEFT;
					}
					else
					{
						if (CounterSpeed == MAX_STACK_POWER )
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
			if (CounterSpeed == MAX_STACK_POWER  && abs(vx) == MARIO_MAX_SPEED * 2)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					if (vx > 0)
						ani = MARIO_ANI_FASTESTRUNRIGHT;
					else
						ani = MARIO_ANI_FASTESTRUNLEFT;
					animation_set->at(ani)->Render(x, y, alpha);
				}
				else if (level == MARIO_LEVEL_SMALL)
				{
					if (vx > 0)
						ani = MARIO_ANI_SMALL_FASTESTRUNRIGHT;
					else
						ani = MARIO_ANI_SMALL_FASTESTRUNLEFT;
					animation_set->at(ani)->Render(x, y, alpha);
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
					animation_set->at(ani)->Render(x, y, alpha);
				}
				else if (level == MARIO_LEVEL_FIRE)
				{
					if (vx > 0)
						ani = MARIO_ANI_FIRE_FASTESTRUNRIGHT;
					else
						ani = MARIO_ANI_FIRE_FASTESTRUNLEFT;
					animation_set->at(ani)->Render(x, y, alpha);
				}
			}
			else
			{
				animation_set->at(ani)->RenderFTime(x, y, 20, alpha);
			}
		}
		if (IsKickKoopas)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_IDLE_KICKKPRIGHT;
				else
					ani = MARIO_ANI_BIG_IDLE_KICKKPLEFT;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_IDLE_KICKKPRIGHT;
				else
					ani = MARIO_ANI_SMALL_IDLE_KICKKPLEFT;
			}
			else if (level == MARIO_LEVEL_RACOON)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACOON_IDLE_KICKKPRIGHT;
				else
					ani = MARIO_ANI_RACOON_IDLE_KICKKPLEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_IDLE_KICKKPRIGHT;
				else
					ani = MARIO_ANI_FIRE_IDLE_KICKKPLEFT;
			}
		}
		if (IsHoldingKoopas)
		{
			if (level == MARIO_LEVEL_SMALL)
			{
				if (IsOnPlatForm)
				{
					if (vx != 0)
					{
						if (nx > 0)
							animation_set->at(MARIO_ANI_SMALL_HOLDKP_WKR)->RenderHDKP(x, y, alpha);
						else
							animation_set->at(MARIO_ANI_SMALL_HOLDKP_WKL)->Render(x, y, alpha);
					}
					else
					{
						if (nx > 0)
							animation_set->at(MARIO_ANI_SMALL_HOLDKP_IDLER)->Render(x, y, alpha);
						else
							animation_set->at(MARIO_ANI_SMALL_HOLDKP_IDLEL)->Render(x, y, alpha);
					}
				}
				else
				{
					if (nx > 0)
						animation_set->at(MARIO_ANI_SMALL_HOLDKP_JMPR)->Render(x, y, alpha);
					else
						animation_set->at(MARIO_ANI_SMALL_HOLDKP_JMPL)->Render(x, y, alpha);
				}
			}
			else if (level == MARIO_LEVEL_BIG)
			{
				if (IsOnPlatForm)
				{
					if (vx != 0)
					{
						if (nx > 0)
							animation_set->at(MARIO_ANI_BIG_WALKING_HOLDKP_RIGHT)->RenderHDKP(x, y, alpha);
						else
							animation_set->at(MARIO_ANI_BIG_WALKING_HOLDKP_LEFT)->Render(x, y, alpha);
					}
					else
						if (nx > 0)
							animation_set->at(MARIO_ANI_BIG_IDLE_HOLDKPRIGHT)->Render(x, y, alpha);
						else
							animation_set->at(MARIO_ANI_BIG_IDLE_HOLDKPLEFT)->Render(x, y, alpha);
				}
				else
				{
						if (nx > 0)
							animation_set->at(MARIO_ANI_BIG_IDLE_HOLDKPJMPRIGHT)->Render(x, y, alpha);
						else
							animation_set->at(MARIO_ANI_BIG_IDLE_HOLDKPJMPLEFT)->Render(x, y, alpha);
				}
			}
			else if (level == MARIO_LEVEL_RACOON)
			{
				if (IsOnPlatForm)
				{
					if (vx != 0)
					{
						if (nx > 0)
							animation_set->at(MARIO_ANI_RACOON_HOLDKP_WKR)->RenderHDKP(x, y, alpha);
						else
							animation_set->at(MARIO_ANI_RACOON_HOLDKP_WKL)->Render(x, y, alpha);
					}
					else
					{
						if (nx > 0)
							animation_set->at(MARIO_ANI_RACOON_HOLDKP_IDLER)->Render(x, y, alpha);
						else
							animation_set->at(MARIO_ANI_RACOON_HOLDKP_IDLEL)->Render(x, y, alpha);
					}
				}
				else
				{
					if (nx > 0)
						animation_set->at(MARIO_ANI_RACOON_HOLDKP_JMPR)->Render(x, y, alpha);
					else
						animation_set->at(MARIO_ANI_RACOON_HOLDKP_JMPL)->Render(x, y, alpha);
				}
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (IsOnPlatForm)
				{
					if (vx != 0)
					{
						if (nx > 0)
							animation_set->at(MARIO_ANI_FIRE_WALKING_HOLDKP_RIGHT)->RenderHDKP(x, y, alpha);
						else
							animation_set->at(MARIO_ANI_FIRE_WALKING_HOLDKP_LEFT)->Render(x, y, alpha);
					}
					else
						if (nx > 0)
							animation_set->at(MARIO_ANI_FIRE_IDLE_HOLDKPRIGHT)->Render(x, y, alpha);
						else
							animation_set->at(MARIO_ANI_FIRE_IDLE_HOLDKPLEFT)->Render(x, y, alpha);
				}
				else
				{
					if (nx > 0)
						animation_set->at(MARIO_ANI_FIRE_IDLE_HOLDKPJMPRIGHT)->Render(x, y, alpha);
					else
						animation_set->at(MARIO_ANI_FIRE_IDLE_HOLDKPJMPLEFT)->Render(x, y, alpha);
				}
			}
		}
		else if (IsAttack && level == MARIO_LEVEL_RACOON)
		{
		if (nx > 0)
			animation_set->at(ani)->RenderATKR(x, y, alpha);
		else
			animation_set->at(ani)->RenderATK(x, y, alpha);
		}
		else
		{
			animation_set->at(ani)->Render(x, y, alpha);
		}
	for (int i = 0; i < firebullet.size(); i++)
	{
		if (firebullet[i]->Attack)
		firebullet.at(i)->Render();
	}
	tail->Render();
	RenderBoundingBox();
	for (int i = 0; i < effects.size(); i++)
		effects[i]->Render();
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
				if (!IsRollBack)
				{
					IsRollBack = true;
					vx -= MARIO_ACCLERATION;
				}
				SetState(MARIO_STATE_IDLE);
			}
			else
			{
				if (IsRollBack)
					IsRollBack = false;
				nx = 1;
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
				if (!IsRollBack)
				{
					IsRollBack = true;
					vx += MARIO_ACCLERATION;
				}
				SetState(MARIO_STATE_IDLE);
			}
			else
			{
				if (IsRollBack)
					IsRollBack = false;
				nx = -1;
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
		}
		break;
	case MARIO_STATE_JUMP:
			if (IsOnPlatForm)
			{
				vy = -MARIO_JUMP_SPEED_Y;
				IsOnPlatForm = false;
			}
		break;
	case MARIO_STATE_IDLE:
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
			vx -= min(abs(vx), MARIO_FRICTION) * nx;
		}
		break;
	case MARIO_STATE_RUN:
		if (vx != 0)
		{
			IsRunning = true;
		}
		break;
	case MARIO_STATE_FLY:
		if ((CounterSpeed == MAX_STACK_POWER  && level == MARIO_LEVEL_RACOON && abs(vx) == MARIO_MAX_SPEED*2)||IsFlying)
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
		}
		break;
	case MARIO_STATE_ATTACK:
		IsAttack = true;
		TAttack = GetTickCount64();		
		if (level == MARIO_LEVEL_RACOON && !IsSitting)
		{
			tail->IsActivated = true;
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
		break;
	case MARIO_STATE_DROPKOOPAS:
		if (IsHoldingKoopas)
		{
			IsHoldingKoopas = false;
			if (nx > 0)
				KP->x += 4;
			else
				KP->x -= 4;
			KP->IsHolding = false;
			KP->nx = nx;
			KP->SetState(KOOPAS_STATE_HIDDEN_MOVE);
		}
		break;
	case MARIO_STATE_FINISH:
		vx = vy = 0;
		EndScene = true;
		Endscene = GetTickCount64();
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (level != MARIO_LEVEL_SMALL)
	{
		if (level != MARIO_LEVEL_RACOON)
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
		else {
			if (IsSitting)
			{
				left = x + 6;
				bottom = y + MARIO_SET_SITTING_Y;
				right = left + MARIO_BIG_BBOX_WIDTH;
			}
			else {
				if (nx > 0) {
					left = x + 6;
				}
				else
					left = x + 6.5;
				right = left + MARIO_BIG_BBOX_WIDTH;
				bottom = top + MARIO_BIG_BBOX_HEIGHT;
			}
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
	IsHoldingKoopas = false;
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
	SetPosition(Camera::GetInstance()->cam_x+32, start_y);
	SetSpeed(0, 0);
}

