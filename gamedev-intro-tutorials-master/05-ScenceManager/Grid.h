#pragma once
#include "Game.h"';
#include"GameObject.h"
#include "Camera.h"
#define CELL_WIDTH	150
#define CELL_HEIGHT	115
class Grid
{


	int cellWidth;
	int cellHeight;

	int numRow;
	int numCol;

	vector<vector<vector<LPGAMEOBJECT>>>Cell;
public:
	void UpdateGrid(vector<LPGAMEOBJECT> listObj);
	Grid();
	~Grid() {};
	void GetListObj(vector<LPGAMEOBJECT>& listNotMoveObj, vector<LPGAMEOBJECT>& listEnemy, vector<LPGAMEOBJECT>& listObj);
	void InsertObj(LPGAMEOBJECT Object, int Left, int Top, int Right, int Bottom)
	{
		if (Right < 20 && Bottom < 7)
		{
			for (int i = Left; i < Right; i++)
			{
				for (int j = Top; j < Bottom; j++)
				{
					int Type = Object->ObjType;
					Cell[i][j].push_back(Object);
				}
			}
		}
	}
	
};

