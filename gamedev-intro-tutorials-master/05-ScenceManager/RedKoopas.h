#pragma once
#include"Koopas.h"
#include "RedKoopasAI.h"
class RedKoopas :
	public CKoopas
{
public:
	int Type;
	RedKoopasAI* KPAI;
	RedKoopas()
	{
		IsMovingObject = true;
		ObjType = ObjType::REDKOOPAS;
		SetState(KOOPAS_STATE_WALKING);
		KPAI = new RedKoopasAI();
		IsHolding = false;
		nx = 1;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render()
	{
		CKoopas::Render();
		KPAI->RenderBoundingBox();
	}
};

