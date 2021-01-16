#pragma once
#include "GameObject.h"
#include "FireBullet.h"
#include "Koopas.h"
#include "Tail.h"
#include "Score.h"
#include "Effect.h"
#include "WarpPipe.h"
#define MARIO_WALKING_SPEED			0.15f 
#define MARIO_JUMP_SPEED_Y			0.25f
#define MARIO_JUMP_DEFLECT_SPEED	0.2f
#define MARIO_DIE_DEFLECT_SPEED		0.2f
#define MARIO_ACCLERATION			0.004f
#define MARIO_DECLERATION			0.007f
#define MARIO_MAX_SPEED				0.15f
#define MARIO_FRICTION				0.003f
#define MARIO_FLY_SPEED				0.25f
#define MARIO_MAX_HIGH_JUMP			70
#define MARIO_SET_LEVEL_Y			19
#define MARIO_SET_IDLE_Y			9
#define MARIO_SET_SITTING_Y			18



#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	1
#define MARIO_STATE_WALKING_LEFT	2
#define MARIO_STATE_JUMP			3
#define MARIO_STATE_DIE				4
#define MARIO_STATE_SIT				5
#define MARIO_STATE_RUN				6
#define MARIO_STATE_FLY				7
#define MARIO_STATE_SLOWFALLING     8
#define MARIO_STATE_ATTACK          9
#define MARIO_STATE_GO_HIDDENMAP	10
#define MARIO_STATE_GO_OUT_HIDDENMAP	11



/*ANI_MARIO_BIG*/
#define MARIO_ANI_DIE					0	
#define MARIO_ANI_BIG_IDLE_RIGHT		1
#define MARIO_ANI_BIG_IDLE_LEFT			2
#define MARIO_ANI_BIG_WALKING_RIGHT			5
#define MARIO_ANI_BIG_WALKING_LEFT			6
#define MARIO_RB_WALKINGRIGHT				8
#define MARIO_RB_WALKINGLEFT				7
#define MARIO_ANI_JUMPINGRIGHT				9
#define MARIO_ANI_JUMPINGLEFT				10
#define MARIO_ANI_FALLINGRIGHT				11
#define MARIO_ANI_FALLINGLEFT				12
#define MARIO_ANI_FASTESTRUNRIGHT			13
#define MARIO_ANI_FASTESTRUNLEFT			14	
#define MARIO_ANI_SITTINGRIGHT				15
#define MARIO_ANI_SITTINGLEFT				16
#define MARIO_ANI_FLYJUMPRIGHT				17
#define MARIO_ANI_FLYJUMPLEFT				18
#define MARIO_ANI_BIG_IDLE_HOLDKPRIGHT		63
#define MARIO_ANI_BIG_WALKING_HOLDKP_RIGHT	64	
#define MARIO_ANI_BIG_KICKKP_RIGHT			65
#define MARIO_ANI_BIG_IDLE_HOLDKPLEFT		66
#define MARIO_ANI_BIG_WALKING_HOLDKP_LEFT	67
#define MARIO_ANI_BIG_KICKKP_LEFT			68
#define MARIO_ANI_BIG_IDLE_HOLDKPJMPRIGHT	81
#define MARIO_ANI_BIG_IDLE_HOLDKPJMPLEFT	82

/*ANI_MARIO_SMALL*/
#define MARIO_ANI_SMALL_IDLE_RIGHT			3
#define MARIO_ANI_SMALL_IDLE_LEFT			4
#define MARIO_ANI_SMALL_WALKING_RIGHT		19
#define MARIO_ANI_SMALL_WALKING_LEFT		20
#define	MARIO_ANI_SMALLJUMPRIGHT			21
#define	MARIO_ANI_SMALLJUMPLEFT				22
#define MARIO_ANI_SMALL_ROLLBACKRIGHT		23
#define	MARIO_ANI_SMALL_ROLLBACKLEFT		24
#define MARIO_ANI_SMALL_FASTESTRUNRIGHT		25
#define MARIO_ANI_SMALL_FASTESTRUNLEFT		26	
#define MARIO_ANI_SMALL_HOLDKP_IDLER		75
#define MARIO_ANI_SMALL_HOLDKP_WKR 			77
#define MARIO_ANI_SMALL_HOLDKP_IDLEL		76
#define MARIO_ANI_SMALL_HOLDKP_WKL 			78
#define MARIO_ANI_SMALL_HOLDKP_JMPR			79
#define MARIO_ANI_SMALL_HOLDKP_JMPL			80
	
#define MARIO_ANI_RACOON_IDLE_RIGHT			27
#define MARIO_ANI_RACOON_IDLE_LEFT			28
#define MARIO_ANI_RACOON_WALKING_RIGHT		29
#define MARIO_ANI_RACOON_WALKING_LEFT		30
#define MARIO_ANI_RACOON_RBWALKINGRIGHT		31
#define MARIO_ANI_RACOON_RBWALKINGLEFT		32
#define MARIO_ANI_RACOON_JUMPINGRIGHT		33
#define MARIO_ANI_RACOON_JUMPINGLEFT		34
#define MARIO_ANI_RACOON_FALLINGRIGHT		35
#define MARIO_ANI_RACOON_FALLINGLEFT		36
#define MARIO_ANI_RACOON_FASTESTRUNRIGHT	37
#define MARIO_ANI_RACOON_FASTESTRUNLEFT		38	
#define MARIO_ANI_RACOON_SITTINGRIGHT		39
#define MARIO_ANI_RACOON_SITTINGLEFT		40
#define MARIO_ANI_RACOON_FLYRIGHT			41
#define MARIO_ANI_RACOON_FLYLEFT			42
#define MARIO_ANI_RACOON_SLOWFALLRIGHT		43
#define MARIO_ANI_RACOON_SLOWFALLLEFT		44
#define MARIO_ANI_RACOON_ATTACKRIGHT		45
#define MARIO_ANI_RACOON_ATTACKLEFT			46
#define MARIO_ANI_RACOON_HOLDKP_IDLER		69
#define MARIO_ANI_RACOON_HOLDKP_WKR 		70
#define MARIO_ANI_RACOON_HOLDKP_IDLEL		71
#define MARIO_ANI_RACOON_HOLDKP_WKL 		72
#define MARIO_ANI_RACOON_HOLDKP_JMPR		73
#define MARIO_ANI_RACOON_HOLDKP_JMPL		74

#define MARIO_ANI_FIRE_IDLE_RIGHT			47
#define MARIO_ANI_FIRE_IDLE_LEFT			48
#define MARIO_ANI_FIRE_WALKING_RIGHT		49
#define MARIO_ANI_FIRE_WALKING_LEFT			50
#define MARIO_ANI_FIRE_RBWALKINGRIGHT		51
#define MARIO_ANI_FIRE_RBWALKINGLEFT		52
#define MARIO_ANI_FIRE_JUMPINGRIGHT			53
#define MARIO_ANI_FIRE_JUMPINGLEFT			54
#define MARIO_ANI_FIRE_FALLINGRIGHT			55
#define MARIO_ANI_FIRE_FALLINGLEFT			56
#define MARIO_ANI_FIRE_FASTESTRUNRIGHT		57
#define MARIO_ANI_FIRE_FASTESTRUNLEFT		58	
#define MARIO_ANI_FIRE_SITTINGRIGHT			59
#define MARIO_ANI_FIRE_SITTINGLEFT			60
#define MARIO_ANI_FIRE_FLYJUMPRIGHT			61
#define MARIO_ANI_FIRE_FLYJUMPLEFT			62
#define MARIO_ANI_FIRE_IDLE_HOLDKPRIGHT		83
#define MARIO_ANI_FIRE_WALKING_HOLDKP_RIGHT	84	
#define MARIO_ANI_FIRE_IDLE_HOLDKPLEFT		85
#define MARIO_ANI_FIRE_WALKING_HOLDKP_LEFT	86
#define MARIO_ANI_FIRE_IDLE_HOLDKPJMPRIGHT	87
#define MARIO_ANI_FIRE_IDLE_HOLDKPJMPLEFT	88



/*MARIO_LEVEL*/
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACOON	3
#define MARIO_LEVEL_FIRE	4

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_RACOON_BBOX_WIDTH  14
#define MARIO_RACOON_BBOX_HEIGHT 29


#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000
#define MARIO_CANFLY_TIME 5000
#define MARIO_ROLLBACK_TIME 100
#define MARIO_PLUSSTACK_TIME 200
#define MARIO_RACOON_ATKTIME 250

#define HIDDEN_MAP_X	2096
#define HIDDEN_MAP_Y	464
#define GO_OUT_HIDDEN_MAP_X	2330
#define GO_OUT_HIDDEN_MAP_Y	352

#define MAX_STACK_POWER 7
#define MAP_MAX_WIDTH	2816
class CMario : public CGameObject
{
	
	int untouchable;
	DWORD untouchable_start;
	float StartJump;
	float start_x;			// initial position of Mario at scene
	float start_y; 
	float LastVx;
public:
	int level;
	int CounterSpeed;
	bool IsOnPlatForm,
		IsRollBack,
		IsSitting,
		IsRunning,
		IsFlying,
		IsEndRollBack,
		IsSlowFalling,
		IsAttack,
		IsHoldingKoopas;
	CKoopas* KP;
	vector<FireBullet*> firebullet;
	int AmountofFirebullet;
	int ani;
	DWORD TRollBack = 0;
	DWORD TPlusStack = 0;
	DWORD TCanFly = 0;
	DWORD TAttack = 0;
	Tail* tail = new Tail();
	bool IsCollision,IsCollisionBackUp;
	bool GoHiddenMap,
		 GoOutHiddenMap,
		 StopUpdate;
	bool IsSwitchScene;
	int NextSceneID;
	float StartYgoHiddenMap;
	Score * score;
	vector<Effect*> effects;
	WarpPipe* WP = new WarpPipe();
public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Calclevel();
	void Reset();
	void AdoptPosHolding()
	{
			KP->SetSpeed(vx, 0);
	}
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void StopRunning()
	{
		IsRunning = false;
		if (IsHoldingKoopas) {
			KP->nx = this->nx;
			KP->SetState(KOOPAS_STATE_HIDDEN_MOVE);
			IsHoldingKoopas = false;
		}
	}
	void IncreaseStack()
	{
		if (CounterSpeed < 7 && (GetTickCount64() - TPlusStack >= MARIO_PLUSSTACK_TIME))
		{
			TPlusStack = 0;
			CounterSpeed++;
		}
	}
	void DecreaseStack()
	{
		if (CounterSpeed > 0 && (GetTickCount64() - TPlusStack >= 100))
		{
			TPlusStack = 0;
			CounterSpeed--;
		}
	}
	
};