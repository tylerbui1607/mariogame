#include "QuestionBrick.h"
#include "Utils.h"
void QuestionBrick::Render()
{
	if (!Check)
		animation_set->at(ANI_BRICK)->Render(x, y);
	else
		animation_set->at(ANI_BRICK_TWO)->Render(x, y);
	RenderBoundingBox();
}

void QuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{

	if (!Check)
	{
		if (vy == 0)
			StartY = y;
		if (StartY - y >= MAX_HIGH)
		{
			vy = BRICK_SPEED_Y;
		}
		if (StartY - y < 0)
		{
			SetState(BRICK_STATE_NOTHINGLEFT);
		}
		CGameObject::Update(dt);
		y += dy;
	}
}

void QuestionBrick::SetState(int state)
{
	if (!Check)
	{
	switch (state)
	{
	case BRICK_STATE_COLISSION:
		vy = -BRICK_SPEED_Y;
		IsHit = true;
		break;
	case BRICK_STATE_NOTHINGLEFT:		
		vy = 0;
		if (y > StartY)
			y = StartY;
		if (Health == 2)
			Check = true;
		SubHealth();
		break;
	}
	}
}


