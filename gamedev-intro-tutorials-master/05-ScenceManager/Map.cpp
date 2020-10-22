#include "Map.h"
#include"GameObject.h"
#include"Utils.h"
Map::Map()
{
	CTextures::GetInstance()->Add(300, L"textures/map1.png", D3DCOLOR_XRGB(255, 255, 255));
	ReadMap();
}
void Map::ReadMap()
{
	ifstream ifs("textures/map1.txt", ios::in);
	ifs >> row >> collum >> TileRow >> TileCollum>>TileWidth;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < collum; j++)
		{
			ifs >> TiledID[i][j];
		}
	}
	ifs.close();
}
void Map::DrawMap()
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < collum; j++)
		{
			int Id = TiledID[i][j];
			RECT r;
			r.left = (Id - 1) % TileCollum * TileWidth;
			if (Id % TileRow == 0)
				r.top = (Id / TileRow - 1) * TileWidth;
			else
				r.top = Id /TileRow * 16;
			r.right = r.left + TileWidth;
			r.bottom = r.top + TileWidth;
			//DebugOut(L"%d\n",Id);
			CGame::GetInstance()->Draw(j * 16, i * 16, CTextures::GetInstance()->Get(300), r.left, r.top, r.right, r.bottom);
		}
}
