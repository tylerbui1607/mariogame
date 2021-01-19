#pragma once
#include "GameObject.h"
class RedKoopasAI :public CGameObject
{
public:
	bool isFalling = false;
public:
	RedKoopasAI() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* listMapObj);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

