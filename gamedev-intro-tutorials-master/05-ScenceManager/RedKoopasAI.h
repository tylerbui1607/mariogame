#pragma once
#include "GameObject.h"
class RedKoopasAI :
	public CGameObject
{

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) 
	{
		left = x;
		top = y;
		right = left + 16;
		bottom = top + 16;
	};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render()
	{
		RenderBoundingBox();
	};
	void AdaptKoopasPos(int KPnx, float KPx, float KPy,float )
	{
		if (nx > 0)
			x = KPx + 16;
		/*else
			x=*/
	};
	virtual void SetState(int state) { this->state = state; }
};

