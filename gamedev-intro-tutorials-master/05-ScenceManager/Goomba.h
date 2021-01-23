#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f;

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHTLV2 24

#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIEBYTAIL	300
#define GOOMBA_STATE_FLY	400
#define GOOMBA_STATE_READY_FLY	500

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1
#define GOOMBA_ANI_WALKING_HAVEWINGS	2
#define GOOMBA_ANI_READY_FLY	3
#define GOOMBA_ANI_FLYING	4

#define GOOMBA_LEVEL_NORMAL 1
#define GOOMBA_LEVEL_FLY	2	


class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public: 
	int Level;
	bool IsDying;
	bool IsFlyGoomba,
		IsFlying, ReadyFly, IsDie;
	int count;
	DWORD WalkingTime;
	DWORD DyingTime;
	CGoomba();
	CGoomba(int level);
	virtual void SetState(int state);
};