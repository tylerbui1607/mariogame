#pragma once
#include "GameObject.h"

#define CURTAIN_ANIMATION	0
class Curtain :
	public CGameObject
{
public:
	bool IsUp, IsDown, StopRender, IsAppear, choose;
	DWORD AppearTime;
	Curtain* ct;
	Curtain() {
		choose = StopRender = IsAppear = IsDown = IsUp = false;
	};
	Curtain(float X, float Y)
	{
		StopRender = true;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	};
	virtual void SetState(int state);
};

