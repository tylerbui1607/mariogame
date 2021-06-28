#include "Tail.h"
#include"QuestionBrick.h"
#include "Brick.h"
#include"Koopas.h"
#include "RedKoopas.h"
#include "Goomba.h"
void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < effects.size(); i++)
		effects[i]->Update(dt);
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
					{
						brick->SetState(BRICK_STATE_BROKEN);
					}
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
			if (coObjects->at(i)->ObjType == ObjType::FIREPIRANHAPLANT)
			{
				if (CheckAABB(coObjects->at(i)))
					coObjects->at(i)->SubHealth();
			}
		}
	}
}


void Tail::Render()
{
	RenderBoundingBox();

}
