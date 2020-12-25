#include "Tail.h"
#include"QuestionBrick.h"
void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (IsActivated)
	{
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->ObjType == ObjType::KOOPAS || coObjects->at(i)->ObjType == ObjType::GOOMBA)
			{
				if (CheckAABB(coObjects->at(i))) {
					coObjects->at(i)->SubHealth();
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
	if (IsActivated)
		RenderBoundingBox();
}
