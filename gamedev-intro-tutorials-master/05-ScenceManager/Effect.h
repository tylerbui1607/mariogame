#pragma once
#include "Game.h"
#include "Animations.h"
#include "Define.h"
#include "Utils.h"
#include "Game.h"
#define EFFECT_SCORE 40
#define EFFECT_BRICK_BREAK 50

#define SCOREEFFECT_RENDER_TIME	1000
class Effect
{
public:
	int EffectType;
	float x, y, vx, vy;
	int AnimationID;
	bool StopRender;
	bool IsBrickEffect;
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
		IsBrickEffect = false;
	};
	void Update(DWORD dt)
	{
		if (GetTickCount64() - RenderTime >= SCOREEFFECT_RENDER_TIME)
		{
			StopRender = true;
		}
		if (IsBrickEffect)
			vy += 0.0009f * dt;
		x += vx * dt;
		y += vy * dt;
	}
	void SetSpeed(float Vx, float Vy)
	{
		vx = Vx;
		vy = Vy;
	}
	virtual void Render()
	{
		//if (!StopRender)
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
		case EFFECT_BRICK_BREAK:
			IsBrickEffect = true;
			break;
		}
	}
};

