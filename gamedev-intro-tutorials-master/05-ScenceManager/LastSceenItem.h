#pragma once
#include "GameObject.h"
#define STATE_ACTIVE	100
class LastSceenItem :
	public CGameObject
{
public:
	int Item;
	bool IsActive;
	LastSceenItem() {
		int Item = 0;
		IsActive = false;
		ObjType = ItemType::LASTITEM;
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		Item = 0;
		IsActive = false;
		left = x; 
		top = y;
		right = left + 16;
		bottom = top + 16;
	};
	void CalcLastItem()
	{
		if (IsActive)
		{
			srand(time(NULL));
			Item = rand() % (3)+1;
		}
	};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
};

