#include"Textures.h"
#include <iostream>
#include <fstream>
#include "Game.h"
#include"Utils.h"
class Map
{
	int collum;
	int row;
	int TiledWidth;
	int TiledID[200][200];
	CTextures* textures = new CTextures();
public:
	Map();
	void ReadMap();
	void DrawMap();
	~Map();
};


