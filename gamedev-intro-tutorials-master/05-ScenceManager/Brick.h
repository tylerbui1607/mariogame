#pragma once
#include "GameObject.h"
#include "Utils.h"

#define Mario
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
#define BRICK_STATE_CHANGETOCOIN 10
#define BRICK_STATE_CHANGEBACKBRICK 11
#define BRICK_STATE_COLLISION 12

class CBrick : public CGameObject
{
public:
	CBrick(int Item)
	{
		ObjType = ObjType::BRICK;
		ItemType=Item;
		if (ItemType != 0)
			Health = 2;
	};
	int ItemType;
	bool Check;
	bool IsCollision;
	DWORD LimitTime;
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {
		if (!IsCollision) {
			if (Check)
			{
				if (GetTickCount64() - LimitTime >= 8000)
				{
					SetState(BRICK_STATE_CHANGEBACKBRICK);
				}
			}
		}
	}
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
};