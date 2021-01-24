#pragma once
#include "GameObject.h"
#include "Boomerang.h"
#include "BomerangBrother.h"
#define BROTHER_MOVING_ATTACK				 0
#define BROTHER_MOVING_RETURN				 1
#define BOMERANGBROTHER_STATE_READY_TO_FIGHT 2
#define BOMERANGBROTHER_STATE_DIE			 3



#define BOMERANGBROTHER_BBOX_WIDTH 16
#define BOMERANGBROTHER_BBOX_HEIGHT 22

#define DISTANCE_GO_AROUND			50


class CBomerangBrother :public CGameObject
{
public:
	float  minX;
	float maxX;
	vector<LPGAMEOBJECT> listBomerang;
	int countBoomerang = 0;
	DWORD timeStartFight = 0;
	bool isCreateBoomerang = false;
public:
	CBomerangBrother(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* listMapObj);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CreateBoomerang();
	void SetState(int state);
};