#include "LastSceenItem.h"
#include <cstdlib>
#include <ctime>

void LastSceenItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (IsActive)
		SubHealth();
}

void LastSceenItem::Render()
{
	if (!IsActive)
	{
		animation_set->at(0)->Render(x, y);
	}
}

void LastSceenItem::SetState(int state)
{
	switch (state)
	{
	case STATE_ACTIVE:
		IsActive = true;
		break;
	default:
		break;
	}
}
