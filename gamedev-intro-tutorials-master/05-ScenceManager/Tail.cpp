#include "Tail.h"
#include"QuestionBrick.h"
#include "Brick.h"
#include"Koopas.h"
#include "RedKoopas.h"
#include "Goomba.h"
void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (IsActivated)
	{
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->ObjType == ObjType::KOOPAS|| coObjects->at(i)->ObjType == ObjType::REDKOOPAS)
			{
				if (CheckAABB(coObjects->at(i))) {
					coObjects->at(i)->nx = nx;
					coObjects->at(i)->SetState(KOOPAS_STATE_DIEBYTAIL);
				}
			}
			if (coObjects->at(i)->ObjType == ObjType:: GOOMBA)
			{
				if (CheckAABB(coObjects->at(i)))
				{
					coObjects->at(i)->nx = nx;
					coObjects->at(i)->SetState(GOOMBA_STATE_DIEBYTAIL);
				}
			}
			if (coObjects->at(i)->ObjType == ObjType::BRICK)
			{
				if (CheckAABB(coObjects->at(i))) {
					CBrick* brick = dynamic_cast<CBrick*>(coObjects->at(i));
					if (brick->ItemType != ObjType::BUTTON)
						coObjects->at(i)->SubHealth();
					else
						brick->SetState(BRICK_STATE_COLLISION);
				}
			}
			if (coObjects->at(i)->ObjType == ObjType::QUESTIONBRICK)
			{
				if (CheckAABB(coObjects->at(i))) {
					coObjects->at(i)->SetState(BRICK_STATE_COLISSION);
				}
			}
		}
	}
}


void Tail::Render()
{
	if (!StopRender && !Attacking)
	{
		if (Idling)
		{
			if (nx > 0)
				ani = 4;
			else
				ani = 5;
		}
		else if (Walking)
		{
			if (nx > 0)
				ani = 6;
			else
				ani = 7;
		}
		else if (Jumping)
		{
			if (nx > 0)
				ani = 2;
			else
				ani = 3;
		}
		else if (Falling)
		{
			if (nx > 0)
				ani = 0;
			else
				ani = 1;
		}
		else if (SlowFalling)
		{
			if (nx > 0)
				ani = 10;
			else
				ani = 11;
		}
		if (Attacking)
		{
			if (nx > 0)
				ani = 8;
			else
				ani = 9;
		}
		if (ani == 9)
			animation_set->at(ani)->RenderATK(x, y);
		else
			animation_set->at(ani)->Render(x, y);
	}
}
