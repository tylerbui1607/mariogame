#pragma once
#include "GameObject.h"
class Coin :
	public CGameObject
{
	float StartY;
	bool IsActivated;
public:
	Coin(float X, float Y)
	{
		x = X + 3;
		StartY = y = Y;
		vy = -0.2;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
	}
};
