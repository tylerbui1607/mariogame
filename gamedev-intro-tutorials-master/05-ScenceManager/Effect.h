#pragma once
#include "Game.h"
#include "Animations.h"

class Effect
{
public:
	float x, y;
	int AnimationSetID;
	int AnimationID;
	static Effect* __instance;

	static Effect* Effect::GetInstance()
	{
		if (__instance == NULL) __instance = new Effect();
		return __instance;
	}
	Effect();
	Effect(float effectX, float effectY, int AnimationSet, int effectType)
	{
		x = effectX;
		y = effectY;
		AnimationSetID = AnimationSet;
		AnimationID = effectType;
	};
	virtual void Render()
	{
		CAnimationSets::GetInstance()->Get(AnimationSetID)->at(AnimationID)->Render(x, y);
	};
};

