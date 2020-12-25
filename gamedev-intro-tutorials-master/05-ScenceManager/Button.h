#pragma once
#include "GameObject.h"
#include "Brick.h"
#define	BBOX_WIDTH	16;
#define BBOX_HEIGHT	16	

#define	BUTTON_STATE_INNIT	1
class Button :
	public CGameObject
{
public:
	float StartY;
	Button(float X, float Y)
	{
		x = X;
		StartY = y = Y;
		ObjType = ObjType::BUTTON;
	}
	bool IsPressed;
	vector<LPGAMEOBJECT>Bricks;
	void SetBrickToCoin()
	{
		for (int i = 0; i < Bricks.size(); i++)
		{
			CBrick* brick = dynamic_cast<CBrick*>(Bricks[i]);
			brick->Check = true;
		}
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + BBOX_WIDTH;
		bottom = top + BBOX_HEIGHT;
	};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};

