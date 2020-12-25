#pragma once
#include "GameObject.h"
#define LEAF_BBOX_WIDTH	16
#define LEAF_BBOX_HEIGHT	14

class Leaf :
	public CGameObject
{
	float StartY;
public:
	Leaf(float X, float Y) {
		x = X;
		StartY = y = Y;
		vy = -0.1;
		ObjType = 20;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + LEAF_BBOX_WIDTH;
		bottom = top + LEAF_BBOX_HEIGHT;
	}
};

