#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "Effect.h"
#define Mario
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
#define BRICK_STATE_CHANGETOCOIN 10
#define BRICK_STATE_CHANGEBACKBRICK 11
#define BRICK_STATE_COLLISION 12
#define BRICK_STATE_BROKEN	14
class CBrick : public CGameObject
{
public:
	CBrick(int Item,float X,float Y)
	{
		ObjType = ObjType::BRICK;
		ItemType=Item;
		x = X;
		y = Y;
		Effect* ef1 = new Effect(x - 1, y, 20, 0);
		Effect* ef2 = new Effect(x + 8, y, 20, 0);
		Effect* ef3 = new Effect(x - 1, y + 8, 20, 0);
		Effect* ef4 = new Effect(x + 8, y + 8, 20, 0);
		ef1->SetState(EFFECT_BRICK_BREAK);
		ef2->SetState(EFFECT_BRICK_BREAK);
		ef3->SetState(EFFECT_BRICK_BREAK);
		ef4->SetState(EFFECT_BRICK_BREAK);
		ef1->SetSpeed(-0.06, -0.16 * 2);
		ef2->SetSpeed(0.06, -0.16 * 2);
		ef3->SetSpeed(-0.06, -0.16);
		ef4->SetSpeed(0.06, -0.16);
		effects.push_back(ef1);
		effects.push_back(ef2);
		effects.push_back(ef3);
		effects.push_back(ef4);
		if (ItemType != 0)
			Health = 2;
	};
	int ItemType;
	bool Check;
	bool IsCollision;
	bool IsBroken;
	DWORD LimitTime;
	vector<Effect*> effects;
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {
		if (!IsCollision) {
			if (Check)
			{
				if (GetTickCount64() - LimitTime >= 5000)
				{
					SetState(BRICK_STATE_CHANGEBACKBRICK);
				}
			}
		}
		if (IsBroken )
		{
			for (int i = 0; i < effects.size(); i++)
				effects[i]->Update(dt);
			if (GetTickCount64() - LimitTime > 2000)
				SubHealth();
		}
	}
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
};