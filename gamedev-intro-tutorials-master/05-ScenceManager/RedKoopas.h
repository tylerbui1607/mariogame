#pragma once
#include"Koopas.h"
class RedKoopas :
	public CKoopas
{
public:
	int Type;
	RedKoopas()
	{
		IsMovingObject = true;
		ObjType = ObjType::REDKOOPAS;
		SetState(KOOPAS_STATE_WALKING);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

