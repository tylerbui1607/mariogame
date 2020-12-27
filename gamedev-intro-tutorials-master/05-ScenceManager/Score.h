#pragma once
#include <Windows.h>
#include "Utils.h"
class Score 
{
public:
	static Score* __instance;
	static Score* Score::GetInstance()
	{
		if (__instance == NULL) __instance = new Score();
		return __instance;
	}
public:
	int TotalScore;
	int ScoreEarn;
	Score();
	void CalcScoreEarn()
	{
		if (ScoreEarn == 0)
			ScoreEarn = 100;
		else if (ScoreEarn < 1000)
			ScoreEarn = ScoreEarn * 2;
		else
			ScoreEarn = 1000;
		TotalScore += ScoreEarn;
	}
};

