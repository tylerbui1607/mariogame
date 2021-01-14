#pragma once
#include <Windows.h>
class Score 
{
public:
	int TotalScore;
	int ScoreEarn;
	Score();
	void CalcScoreEarn()
	{
		if (ScoreEarn == 800)
		{
			ScoreEarn = 1000;
			TotalScore += ScoreEarn;
			return;
		}
		else if (ScoreEarn < 800)
		{
			ScoreEarn = ScoreEarn * 2;
			if (ScoreEarn == 0)
				ScoreEarn = 100;
			TotalScore += ScoreEarn;
			return;
		}
		else if (ScoreEarn == 1000)
		{
			ScoreEarn = 0;
			TotalScore += ScoreEarn;
			return;
		}
	}
};

