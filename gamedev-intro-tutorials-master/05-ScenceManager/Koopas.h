#pragma once
#include "Koopas.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "Utils.h"
#include "GameObject.h"
#include "RedKoopasAI.h"

#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_HIDDEN_SPEED 0.2f;

#define KOOPAS_BBOX_WIDTH 14
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 14

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_HIDDEN_MOVE 400
#define KOOPAS_STATE_HIDDEN 300
#define KOOPAS_STATE_REBORN	500	
#define KOOPAS_STATE_DIEBYTAIL	600
#define KOOPAS_STATE_DIEBYSHELL	700


#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_HIDDEN 2
#define KOOPAS_ANI_HIDDENMOVE 3
#define KOOPAS_ANI_HIDDEN_DIE	5
#define KOOPAS_ANI_FLYLEFT	6
#define KOOPAS_ANI_FLYRIGHT	7

#define KOOPAS_LEVEL_NORMAL	1
#define KOOPAS_LEVEL_PARAKOOPAS	2	
class CKoopas : public CGameObject
{
public:
	bool IsWalking, 
		 IsHidden,
		 IsJumpUp,
		 IsHolding,
		 IsAttack,
		 IsReborning,
		IsDieByShell,
		IsDead;
	int Level;
	DWORD TimeReborn,
		  TimeStartReborn;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void AdaptPos(float MarioX, float MarioY, int MarioNx)
	{
		if (MarioNx > 0)
			x = MarioX + 12;
		else if (MarioNx < 0)
			x = MarioX - 14;
		y = MarioY + 5;
	}
	void AdaptPosSmall(float MarioX, float MarioY, int MarioNx)
	{
		if (MarioNx > 0)
			x = MarioX + 10;
		else if (MarioNx < 0)
			x = MarioX - 11;
		y = MarioY - 5;
	}
	void ChkIsHolding(bool isHolding=false)
	{
		IsHolding = isHolding;
	}
public:
	CKoopas() {
		ObjType = ObjType::KOOPAS;
		nx = -1;
		SetState(KOOPAS_STATE_WALKING);
		IsMovingObject = true;
		Level = 1;
	};
	CKoopas(int level);
	virtual void SetState(int state);
};