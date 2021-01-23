#pragma once
#include "Textures.h"
#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
#include "HUD_Time.h"
class Hud
{
public:
	static Hud* __instance;

	static Hud* Hud::GetInstance()
	{
		if (__instance == NULL) __instance = new Hud();
		return __instance;
	}
	Hud() {
		MarioMoney = 7;
	};
	float HUDx, HUDy;
	int SpriteHUD;
	int SpriteStack;
	int SpritePower;
	int SpriteItem;
	int MarioStack;
	int MarioMoney;
	vector<int> Item;
	bool CourseClear;
	DWORD Time = 0;
	HUD_Time* hudTime;
	Hud(float x, float y, int SpriteID,int spriteStackID,int SpritePowerID) {
		HUDx = x;
		HUDy = y;
		SpriteHUD = SpriteID;
		SpriteStack = spriteStackID;
		SpritePower = SpritePowerID;
	};
	void Update(DWORD dt);
	void Render(int Score);

};

