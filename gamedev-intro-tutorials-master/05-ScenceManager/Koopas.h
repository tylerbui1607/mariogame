#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_HIDDEN_SPEED 0.2f;

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_HIDDEN_MOVE 400
#define KOOPAS_STATE_HIDDEN 300


#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_HIDDEN 2
#define KOOPAS_ANI_HIDDENMOVE 3

class CKoopas : public CGameObject
{
public:
	bool IsHidden,
		 IsJumpUp,
		 IsHolding,
		 IsAttack;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void ChkIsHolding(bool isHolding=false)
	{
		IsHolding = isHolding;
	}
public:
	CKoopas();
	virtual void SetState(int state);
};