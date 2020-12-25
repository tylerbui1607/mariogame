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
	if (IsPressed)
		SetBrickToCoin();
}

void Button::Render()
{
	RenderBoundingBox();
}

void Button::SetState(int state)
{
	switch (state)
	{
	case BUTTON_STATE_INNIT:
		vy = -0.2;
		break;
	}
}
