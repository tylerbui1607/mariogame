#pragma once
#include "Textures.h"
#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
class HUD_Time
{
public:
	float HUD_TIMEx, HUD_TIMEy;
	DWORD Count;
	int SpriteID[10];
	int LimitTime;
	HUD_Time(float x, float y, int Sprites[]) {
		HUD_TIMEx = x;
		HUD_TIMEy = y;
		for (int i = 0; i < 10; i++)
		{
			SpriteID[i] = Sprites[i];
		}
		LimitTime = 300;
	};
	void Update(DWORD dt);
	void Render(int Score)
	{
		int n = LimitTime;
		int ts = Score;
		for (int i = 2; i >= 0; i--)
		{
			int c = n % 10;
			CSprites::GetInstance()->Get(SpriteID[c])->DrawHUD(HUD_TIMEx + 8 * i, HUD_TIMEy);
			if (n != 0)
				n = n / 10;
		}
		for (int i = 6; i >= 0; i--)
		{
			if (ts != 0)
			{
				int c = ts % 10;
				CSprites::GetInstance()->Get(SpriteID[c])->DrawHUD(49 + 8 * i, HUD_TIMEy);
				ts = ts / 10;
			}
			else
				CSprites::GetInstance()->Get(SpriteID[0])->DrawHUD(49 + 8 * i, HUD_TIMEy);
		}
	};
};

