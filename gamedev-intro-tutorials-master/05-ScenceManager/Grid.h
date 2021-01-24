#pragma once
#include "Game.h"';
#include"GameObject.h"
#include "Camera.h"
#define CELL_WIDTH	126
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
	bool CheckObjPos(float Objx, float Objy)
	{
		return(!(Objx < 0 || Objx>2816 || Objy < 0 || Objy>624));
	}
	void GetListObj(vector<LPGAMEOBJECT>& listNotMoveObj, vector<LPGAMEOBJECT>& listEnemy, vector<LPGAMEOBJECT>& listObj);
	void InsertObj(LPGAMEOBJECT Object, int Left, int Top, int Right, int Bottom)
	{

			for (int i = Left; i < Right; i++)
			{
				for (int j = Top; j < Bottom; j++)
				{
					int type = Object->ObjType;
					Cell[i][j].push_back(Object);
				}
			}
	}
	void UnLoadGrid()
	{
		Cell.clear();
	}
	
};

