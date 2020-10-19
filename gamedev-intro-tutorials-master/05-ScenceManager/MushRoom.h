
#include "GameObject.h"
#define MUSHROOM_GRAVITY			0.0007f
#define MUSHROOM_SPEED_X			0.05f 
#define MUSHROOM_SPEED_y			0.01f 

#define MUSHROOM_ANI	0

#define MUSHROOOM_BBOX	16

#define MUSHROOM_STATE_INIT		1
#define	MUSHROOM_STATE_NORMAL	2
class MushRoom :
	public CGameObject
{
	float StartInitPos;
	bool Initialized;
public:
	MushRoom(int X,int Y)
	{
		ObjType = 8;
		IsMovingObject = true;
		Initialized = false;
		x = X;
		y = Y;
		StartInitPos = Y;
		SetState(MUSHROOM_STATE_INIT);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void CaclVx(int objx)
	{
		if (objx > x)
			vx = -MUSHROOM_SPEED_X;
		else
			vx = MUSHROOM_SPEED_X;
	}
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + MUSHROOOM_BBOX;
		bottom = top + MUSHROOOM_BBOX;
	}
}
;
