#include "WorldMapScene.h"
#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include"Ground.h"
#include "WarpPipe.h"
#include "Block.h"
#include "QuestionBrick.h"
#include"MushRoom.h"
#include "Item.h"
#include "FirePiranhaPlant.h"
#include "FireBullet.h"
#include "Camera.h"
#include "Coin.h"
#include "Leaf.h"
#include "Map.h"
#include "RedKoopas.h"
#define MAP_MAX_WIDTH 2816
using namespace std;
Map* Worldmap;
WorldMapScene::WorldMapScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new WorldMapSceneKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP	7
#define SCENE_SECTION_HUD	8
#define SCENE_SECTION_HUD_TIME	9
#define OBJECT_TYPE_MARIO			0
#define OBJECT_TYPE_BRICK			1
#define OBJECT_TYPE_GOOMBA			2
#define OBJECT_TYPE_KOOPAS			3
#define OBJECT_TYPE_GROUND			4
#define OBJECT_TYPE_WARPPIPE		5
#define OBJECT_TYPE_BLOCK			6
#define OBJECT_TYPE_QUESTIONBRICK	7
#define OBJECT_TYPE_FIREPIRANHAPLANT	8
#define UNKNOW_ITEM	18
#define OBJECT_TYPE_RED_KOOPAS			9


#define MUSHROOM_ANISET_ID	8

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void WorldMapScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}
void WorldMapScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void WorldMapScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void WorldMapScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void WorldMapScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int width, height;
	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ItemType;
	int ani_set_id;
	float MinX, MaxX;
	if (object_type == ObjType::GROUND || object_type == ObjType::WARPPIPE || object_type == ObjType::BLOCK)
	{
		width = atof(tokens[4].c_str());
		height = atof(tokens[5].c_str());
	}
	if (object_type == ObjType::QUESTIONBRICK)
	{
		ItemType = atof(tokens[4].c_str());
	}
	
	int Id = ani_set_id = atoi(tokens[3].c_str());
	if (object_type == ObjType::REDKOOPAS)
	{
		MinX = atof(tokens[4].c_str());
		MaxX = atof(tokens[5].c_str());
	}

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
	Item* item = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new MarioWM(x, y);
		player = (MarioWM*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_GROUND: obj = new Ground(width, height); break;
	case OBJECT_TYPE_WARPPIPE: obj = new WarpPipe(width, height); break;
	case OBJECT_TYPE_BLOCK: obj = new Block(width, height); break;
	case OBJECT_TYPE_QUESTIONBRICK: obj = new QuestionBrick(ItemType); break;
	case OBJECT_TYPE_FIREPIRANHAPLANT: obj = new FirePiranhaPlant(x,y); break;
	case OBJECT_TYPE_RED_KOOPAS: obj = new RedKoopas(MinX, MaxX); break;
		/*case OBJECT_TYPE_ITEM: objects.push_back(item); break;*/
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		CPortal* portal = dynamic_cast<CPortal*>(obj);
		portal->Check[0]= atoi(tokens[7].c_str());
		portal->Check[1] = atoi(tokens[8].c_str());
		portal->Check[2] = atoi(tokens[9].c_str());
		portal->Check[3] = atoi(tokens[10].c_str());
		portal->ID = Id;
		DebugOut(L"HelloId%d\n", portal->ID);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
	/*camera = new Camera();*/
}

void WorldMapScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 9) return; // skip invalid lines - an object set must have at least id, x, y

	int ID = atoi(tokens[0].c_str());


	wstring FilePath_data = ToWSTR(tokens[1]);

	int Map_width = atoi(tokens[2].c_str());
	int Map_height = atoi(tokens[3].c_str());
	int Num_row_read = atoi(tokens[4].c_str());
	int Num_col_read = atoi(tokens[5].c_str());
	int Tile_width = atoi(tokens[6].c_str());
	int Tile_height = atoi(tokens[7].c_str());
	int A = atoi(tokens[8].c_str());
	Worldmap = new Map(ID, FilePath_data.c_str(), Map_width, Map_height, Num_row_read, Num_col_read, Tile_width, Tile_height);
	if (A == 1)
	{
		Worldmap->IsWorldMap = true;
	}
}


void WorldMapScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	Camera::GetInstance()->cam_x = Camera::GetInstance()->cam_y = 0;
	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}
	f.close();
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	CGame::GetInstance()->DoneLoad = true;
}

void WorldMapScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
}

void WorldMapScene::Render()
{
	if (CGame::GetInstance()->DoneLoad)
	Worldmap->Draw();
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
}

/*
	Unload current scene
*/
void WorldMapScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void WorldMapSceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	MarioWM* mario = ((WorldMapScene*)scence)->GetPlayer();
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALK_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALK_LEFT);
	else if (game->IsKeyDown(DIK_DOWN))
		mario->SetState(MARIO_STATE_WALK_DOWN);
	else if (game->IsKeyDown(DIK_UP))
		mario->SetState(MARIO_STATE_WALK_UP);
}

void WorldMapSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	MarioWM* mario = ((WorldMapScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->Choosen = true;
		break;
	case DIK_LEFT:
		mario->SetState(MARIO_STATE_WALK_LEFT);
		break;
	case DIK_RIGHT:
		mario->SetState(MARIO_STATE_WALK_RIGHT);
		break;
	case DIK_UP:
		mario->SetState(MARIO_STATE_WALK_UP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_WALK_DOWN);
		break;
	}
}

void WorldMapSceneKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	MarioWM* mario = ((WorldMapScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_W:
		mario->Choosen = false;
		break;
	}
}
