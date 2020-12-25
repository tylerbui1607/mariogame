#pragma once
#include"Koopas.h"
class RedKoopas :
	public CKoopas
{
public:
	float MAXx, MINx;
	int Type;
	RedKoopas(float Min,float Max)
	{
		IsMovingObject = true;
		ObjType = ObjType::REDKOOPAS;
		SetState(KOOPAS_STATE_WALKING);
		MAXx = Max;
		MINx = Min;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

