#include <iostream>
#include <fstream>

#include "PlayScence.h"
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
#define MAP_MAX_WIDTH	2816
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
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

#define OBJECT_TYPE_MARIO			0
#define OBJECT_TYPE_BRICK			1
#define OBJECT_TYPE_GOOMBA			2
#define OBJECT_TYPE_KOOPAS			3
#define OBJECT_TYPE_GROUND			4
#define OBJECT_TYPE_WARPPIPE		5
#define OBJECT_TYPE_BLOCK			6
#define OBJECT_TYPE_QUESTIONBRICK	7
#define OBJECT_TYPE_FIREPIRANHAPLANT	8


#define MUSHROOM_ANISET_ID	8

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

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
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int width, height;
	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ani_set_id;

	if (object_type == ObjType::GROUND || object_type == ObjType::WARPPIPE || object_type == ObjType::BLOCK)
		{
			width = atof(tokens[4].c_str());
			height = atof(tokens[5].c_str());
		}
	ani_set_id = atoi(tokens[3].c_str());
	

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	Item* item = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_GROUND: obj = new Ground(width,height); break;
	case OBJECT_TYPE_WARPPIPE: obj = new WarpPipe(width, height); break;
	case OBJECT_TYPE_BLOCK: obj = new Block(width, height); break;
	case OBJECT_TYPE_QUESTIONBRICK: obj = new QuestionBrick(); break;
	case OBJECT_TYPE_FIREPIRANHAPLANT: obj = new FirePiranhaPlant(); break;
	/*case OBJECT_TYPE_ITEM: objects.push_back(item); break;*/
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
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
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

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
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
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
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	map = new Map();
	//map->DrawMap();
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coMovingObjects;
	vector<LPGAMEOBJECT> coNotMoveObjects;
	vector<LPGAMEOBJECT> coObjects;


	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->GetHealth() != 0)
		{
			if (objects[i]->ObjType == ObjType::QUESTIONBRICK && objects[i]->GetHealth() == 2)
			{
				MushRoom* mushroom = new MushRoom(objects[i]->x, objects[i]->y);
				mushroom->CaclVx(player->x);
				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set = animation_sets->Get(MUSHROOM_ANISET_ID);
				mushroom->SetAnimationSet(ani_set);
				objects[i]->SubHealth();
				objects.push_back(objects[i]);
				objects[i] = mushroom;
			}
			coObjects.push_back(objects[i]);
			if (objects[i]->IsMovingObject)
				coMovingObjects.push_back(objects[i]);
			else
				coNotMoveObjects.push_back(objects[i]);
		}
	}
	
	/*DebugOut(L"hello%d\n", coObjects.size());*/
	for (size_t i = 0; i < objects.size(); i++)
	{
	
		if (objects[i]->GetHealth() != 0)
		{
			if (objects[i]->ObjType == ObjType::MARIO)
				objects[i]->Update(dt, &coObjects);
			else 
			{
				if (dynamic_cast<FirePiranhaPlant*>(objects[i]))
				{
					FirePiranhaPlant* plant = dynamic_cast<FirePiranhaPlant *>(objects[i]);
					plant->GetEnemyPos(player->x, player->y);
					if (plant->IsAttack && plant->count == 1)
					{
						plant->CalcAtkPos();
						FireBullet* Firebullet = new FireBullet(plant->VxBullet, plant->VyBullet);
						Firebullet->SetPosition(plant->x, plant->y);
						objects.push_back(Firebullet);
						plant->count--;
					}
					DebugOut(L"hello%f\n", plant->VyBullet);
				}
				objects[i]->Update(dt, &coNotMoveObjects);
			}
		}
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	
	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;

	if (cx < 0)
		cx = 0;
	if (cx > MAP_MAX_WIDTH)
		cx = MAP_MAX_WIDTH;
	if (cy < 0)
		cy = 0;
	if (player->y >= 240)
		cy = 240;
		
	CGame::GetInstance()->SetCamPos(cx, cy);
}

void CPlayScene::Render()
{
	//map->DrawMap();
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetHealth() != 0)
			objects[i]->Render();
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{

	case DIK_B: // reset
		mario->SetState(MARIO_STATE_IDLE);
		mario->SetLevel(MARIO_LEVEL_BIG);
		mario->SetPosition(50.0f, 0.0f);
		mario->SetSpeed(0, 0);
		break;
	case DIK_E:
		mario->SetLevel(MARIO_LEVEL_BIG);
	case DIK_SPACE:
		mario->SetState(MARIO_STATE_JUMP);
		if (mario->level == MARIO_LEVEL_RACOON)
		{
			mario->SetState(MARIO_STATE_SLOWFALLING);
			mario->SetState(MARIO_STATE_FLY);
		}
		break;
	case DIK_A:
		if (mario->level == MARIO_LEVEL_RACOON)
			mario->SetState(MARIO_STATE_ATTACK);
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_A:
		mario->StopRunning();
		break;
	case DIK_SPACE:
		mario->IsSlowFalling = false; 
		if (mario->vy < 0)
			mario->vy = 0;
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (game->IsKeyDown(DIK_A))
		mario->SetState(MARIO_STATE_RUN);
	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN))
		mario->SetState(MARIO_STATE_SIT);
	else
		mario->SetState(MARIO_STATE_IDLE);
}