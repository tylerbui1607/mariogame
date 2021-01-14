#pragma once
#include "Game.h"
#include "Animations.h"
#include "Define.h"
#include "Utils.h"
#include "Game.h"
#define EFFECT_SCORE 40
#define SCOREEFFECT_RENDER_TIME	1000
class Effect
{
public:
	int EffectType;
	float x, y, vx, vy;
	int AnimationID;
	bool StopRender;
	DWORD RenderTime;
	Effect()
	{};
	Effect(float effectX, float effectY, int AnimationSet, int AniID)
	{
		x = effectX;
		y = effectY;
		EffectType = AnimationSet;
		AnimationID = AniID;
		StopRender = false;
	};
	void Update(DWORD dt)
	{
		if (GetTickCount64() - RenderTime >= SCOREEFFECT_RENDER_TIME)
		{
			StopRender = true;
		}
		x += vx * dt;
		y += vy * dt;
	}
	virtual void Render()
	{
		if (!StopRender)
			CAnimationSets::GetInstance()->Get(EffectType)->at(AnimationID)->Render(x, y);
	};
	void SetState(int state)
	{
		switch (state)
		{
		case EFFECT_SCORE:
			vy = -0.05;
			vx = 0;
			RenderTime = GetTickCount64();
			break;
		}
	}
};

