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
	float HUDx, HUDy;
	int SpriteHUD;
	int SpriteStack;
	int SpritePower;
	int MarioStack;
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

