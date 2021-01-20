#pragma once
#include "GameObject.h"
#include "Effect.h"
class Coin :
	public CGameObject
{
	Effect* effect;
	float StartY;
	bool IsActivated;
	bool InnitEffect;
public:
	Coin(float X, float Y)
	{
		x = X + 3;
		StartY = y = Y;
		vy = -0.2;
		InnitEffect = false;
		IsMovingObject = true;

	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
	}
};

