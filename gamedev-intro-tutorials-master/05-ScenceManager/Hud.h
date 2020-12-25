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
	HUD_Time* hudTime;
	Hud(float x, float y, int SpriteID) {
		HUDx = x;
		HUDy = y;
		SpriteHUD = SpriteID;
	};
	void Update(DWORD dt);
	void Render();

};

