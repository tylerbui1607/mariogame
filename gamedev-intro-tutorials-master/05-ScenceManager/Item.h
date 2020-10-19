#pragma once
#include "GameObject.h"
class Item :
	public CGameObject
{
public:
	Item()
	{
		ObjType = 8;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL)
	{}
	virtual void Render(){}

	void SetState(int state){}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom){}
};

