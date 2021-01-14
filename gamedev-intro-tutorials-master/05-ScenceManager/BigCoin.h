#pragma once
#include "GameObject.h"
#include "Score.h"
#define BIGCOIN_STATE_COLLECTED 1
#define BIGCOIN_ANI	0
#define BIGCOIN_BBOX 16

class BigCoin:public CGameObject
{
public:
	BigCoin(float Coinx, float Coiny)
	{
		x = Coinx;
		y = Coiny;
		ObjType = ItemType::BIGCOIN;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL)
	{
	};
	virtual void Render()
	{
		animation_set->at(BIGCOIN_ANI)->Render(x, y);
	};
	void SetState(int state)
	{
		switch (state)
		{
		case BIGCOIN_STATE_COLLECTED:
			SubHealth();
			break;
		}
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + BIGCOIN_BBOX;
		bottom = top + BIGCOIN_BBOX;
	}
};

