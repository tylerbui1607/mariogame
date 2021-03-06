#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include"../LoadGrid/ArrangeGrid.h"


class CPlayScene: public CScene
{
protected: 
	CMario *player;					// A play scene has to have player, right? 
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line, int l, int t, int r, int b);
	void _ParseSection_MAP(string line);
	void _ParseSection_HUD(string line);
	void _ParseSection_HUD_TIME(string line);
	void _ParseSection_CAMERA(string line);
public: 
	ArrangeGrid* ArrangeG = new ArrangeGrid();
	int count;
	CPlayScene(int id, LPCWSTR filePath);
	vector<LPGAMEOBJECT> coMovingObjects;
	vector<LPGAMEOBJECT> coNotMoveObjects;
	vector<LPGAMEOBJECT> coObjects;
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

