#pragma once
#include "GameObject.h"
#define BBOX_WIDTH 15
#define BBOX_HEIGHT 15
#define MARIO_STATE_WALK_RIGHT 1
#define MARIO_STATE_WALK_LEFT 2
#define MARIO_STATE_WALK_UP 3
#define MARIO_STATE_WALK_DOWN 4
#define MARIO_STATE_IDLE	5

class MarioWM :
	public CGameObject
{
public:
	int CanGoLeft,
		 CanGoRight,
		 CanGoUp,
		 CanGoDown;
	int IdPortal;
	bool SwitchScene;
	int NextScene;
	MarioWM(float X, float Y)
	{
		x = X;
		y = Y;
		IdPortal = CanGoLeft = CanGoRight = CanGoUp = CanGoDown = 0;
	}
	bool Choosen;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + BBOX_WIDTH;
		bottom = top + BBOX_HEIGHT;
	};
};

