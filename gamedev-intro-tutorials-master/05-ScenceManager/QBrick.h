#pragma once
#include "GameObject.h"

#define ANI_BRICK 0
#define	ANI_BRICK_TWO	1

#define BRICK_SPEED_Y	0.05
#define MAX_HIGH	18
#define BRICK_STATE_COLISSION 1
#define BRICK_STATE_NOTHINGLEFT 2

class QBrick :
	public CGameObject
{
	float StartY;
public:
	bool Check;
	QBrick()
	{
		ObjType = 7;
		Check = false;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = x + 16;
		bottom = y + 16;
	}
};

