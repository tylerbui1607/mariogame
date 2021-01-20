#include "Grid.h"

void Grid::UpdateGrid(vector<LPGAMEOBJECT> listObj)
{
	int left = (int)(Camera::GetInstance()->cam_x / CELL_WIDTH);
	int right = ceil(((Camera::GetInstance()->cam_x + 300 )/ CELL_WIDTH));
	int top = (int)(Camera::GetInstance()->cam_y / CELL_HEIGHT);
	int bottom = ceil(((Camera::GetInstance()->cam_y + 230) / CELL_HEIGHT));
	for (int i = left; i < right; i++)
	{
		for (int j = top; j < bottom; j++)
		{
			Cell[i][j].clear();
		}
	}
	for (int i = 0; i < listObj.size(); i++)
	{
		float l, t, r, b;
		listObj[i]->GetBoundingBox(l, t, r, b);
		int Top = int(t /CELL_HEIGHT);
		int Left = int(l /CELL_WIDTH);
		int Right = ceil(r /CELL_WIDTH);
		int Bottom = ceil(b /CELL_HEIGHT);
		//DebugOut(L"Left%d\n", Left);
		//DebugOut(L"Top%d\n", Top);
		//DebugOut(L"Right%d\n", Right);
		//DebugOut(L"Bottom%d\n", Bottom);
		if (listObj[i]->Health != 0)
		InsertObj(listObj[i], Left, Top, Right, Bottom);
	}
}

Grid::Grid()
{
	numRow = ( 624+ CELL_HEIGHT) /CELL_HEIGHT;
	numCol = (2816+ CELL_WIDTH) /CELL_WIDTH;
	Cell.resize(numCol + 1);
	for (int i = 0; i < numCol + 1; i++)
		Cell[i].resize(numRow + 1);
	for (int i = 0; i < numCol; i++)
		for (int j = 0; j < numRow; j++)
			Cell[i][j].clear();
}

void Grid::GetListObj(vector<LPGAMEOBJECT>& listNotMoveObj, vector<LPGAMEOBJECT>& listEnemy, vector<LPGAMEOBJECT>& listObj)
{
	int left = (int)((Camera::GetInstance()->cam_x )/ CELL_WIDTH);
	int right = ceil(((Camera::GetInstance()->cam_x + 300) / CELL_WIDTH));
	int top = (int)((Camera::GetInstance()->cam_y) / CELL_HEIGHT);
	int bottom = ceil(((Camera::GetInstance()->cam_y + 230) / CELL_HEIGHT));

	listObj.clear();
	listEnemy.clear();
	listNotMoveObj.clear();
	//DebugOut(L"Left%d\n", left);
	//DebugOut(L"Top%d\n", top);
	//DebugOut(L"Right%d\n", right);
	//DebugOut(L"Bottom%d\n", bottom);
	for (int i = left; i < right; i++)
	{
		for (int j = top; j < bottom; j++)
		{
			
			for (int k = 0; k < Cell[i][j].size(); k++)
			{
				if (Cell[i][j][k]->ObjType != ObjType::MARIO && !Cell[i][j][k]->IsInList)
				{
					if (!Cell[i][j][k]->IsMovingObject)
					{
						listNotMoveObj.push_back(Cell[i][j][k]);
						Cell[i][j][k]->IsInList = true;
					}
					else
					{
						listEnemy.push_back(Cell[i][j][k]);
						Cell[i][j][k]->IsInList = true;
					}
					listObj.push_back(Cell[i][j][k]);
				}
			}
		}
	}
}


