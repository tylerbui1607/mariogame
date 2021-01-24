
#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Goomba.h"
#include "Koopas.h"
#include "MarioWM.h"
#include "Hud.h"
	class WorldMapScene : public CScene
	{
	protected:
		MarioWM* player;					// A play scene has to have player, right? 
		vector<LPGAMEOBJECT> objects;

		void _ParseSection_TEXTURES(string line);
		void _ParseSection_SPRITES(string line);
		void _ParseSection_ANIMATIONS(string line);
		void _ParseSection_ANIMATION_SETS(string line);
		void _ParseSection_OBJECTS(string line);
		void _ParseSection_MAP(string line);
		void _ParseSection_HUD(string line);
		void _ParseSection_HUD_TIME(string line);
	public:

		WorldMapScene(int id, LPCWSTR filePath);

		virtual void Load();
		virtual void Update(DWORD dt);
		virtual void Render();
		virtual void Unload();
		MarioWM* GetPlayer() { return player; }

		//friend class CPlayScenceKeyHandler;
	};

	class WorldMapSceneKeyHandler : public CScenceKeyHandler
	{
	public:
		virtual void KeyState(BYTE* states);
		virtual void OnKeyDown(int KeyCode);
		virtual void OnKeyUp(int KeyCode);
		WorldMapSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
	};




