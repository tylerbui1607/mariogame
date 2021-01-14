#include "Brick.h"

void CBrick::Render()
{
	if(IsCollision)
		animation_set->at(1)->Render(x, y);
	else
		animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrick::SetState(int state)
{
	switch (state)
	{
	case BRICK_STATE_CHANGETOCOIN:
		if (ItemType == 0) {
			SetAnimationSet(CAnimationSets::GetInstance()->Get(21));
			ObjType = ItemType::BIGCOIN;
			LimitTime = GetTickCount64();
			Check = true;
			//DebugOut(L"ObjType%d\n", ObjType);
		}
		break;
	case BRICK_STATE_CHANGEBACKBRICK:
		ObjType = ObjType::BRICK;
		SetAnimationSet(CAnimationSets::GetInstance()->Get(2));
		Check = false;
		break;
	case BRICK_STATE_COLLISION:
		IsCollision = true;
		break;
	}
}
