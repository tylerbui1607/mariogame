#include "Button.h"

void Button::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (StartY - y <= 16)
		y += dy;
	else
	{
		vy = 0;
		y = StartY - 16;
	}
}

void Button::Render()
{
	if (IsPressed)
	{
		animation_set->at(1)->Render(x, y);
	}
	else
		animation_set->at(0)->Render(x, y);
}

void Button::SetState(int state)
{
	switch (state)
	{
	case BUTTON_STATE_INNIT:
		vy = -0.2;
		break;
	case BUTTON_STATE_ISPRESSED:
		if (!IsPressed)
		{
			DebugOut(L"HelloButton\n");
			IsPressed = true;
			y += BBOX_HEIGHT - BBOX_HEIGHT_ISPRESSED;
			SetBrickToCoin();
		}
		break;
	}
}
