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
#include "Camera.h"
#include "Coin.h"
#include "Leaf.h"
#include "Hud.h"
#include "Map.h"
#include "RedKoopas.h"
#include "Button.h"
#include "BigCoin.h"
#include "PiranhaPlant.h"
#include "Grid.h"
#define MAP_MAX_WIDTH	 2816
using namespace std;
Camera* camera;
Hud* hud;
Map* map;
bool DoneLoad;
vector<LPGAMEOBJECT>Cbricks;
Grid* grid;
CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	grid = new Grid();
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN	   -1
#define SCENE_SECTION_TEXTURES		2
#define SCENE_SECTION_SPRITES		3
#define SCENE_SECTION_ANIMATIONS	4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS		6
#define SCENE_SECTION_MAP			7
#define SCENE_SECTION_HUD			8
#define SCENE_SECTION_HUD_TIME		9
#define OBJECT_TYPE_MARIO			0
#define OBJECT_TYPE_BRICK			1
#define OBJECT_TYPE_GOOMBA			2
#define OBJECT_TYPE_KOOPAS			3
#define OBJECT_TYPE_GROUND			4
#define OBJECT_TYPE_WARPPIPE		5
#define OBJECT_TYPE_BLOCK			6
#define OBJECT_TYPE_QUESTIONBRICK	7
#define OBJECT_TYPE_FIREPIRANHAPLANT	8
#define UNKNOW_ITEM					18
#define OBJECT_TYPE_RED_KOOPAS		9
#define OBJECT_TYPE_BUTTON			10
#define OBJECT_TYPE_UNBREAK_BRICK	11
#define OBJECT_TYPE_BIGCOIN			21
#define OBJECT_TYPE_PIRANHAPLANT	22


#define MUSHROOM_ANISET_ID	8

#define OBJECT_TYPE_PORTAL	50

#define OBJECT_TYPE_PORTAL_LASTSCENE	500

#define MAX_SCENE_LINE 1024

#define BIGCOIN_ANISET_ID	21
#define LEAF_ANISET_ID		14

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
	int ItemType;
	int ani_set_id;
	int Level;
	if (object_type == ObjType::KOOPAS || object_type == ObjType::GOOMBA)
		Level = atoi(tokens[4].c_str());
	if (object_type == ObjType::GROUND || object_type == ObjType::WARPPIPE || object_type == ObjType::BLOCK)
		{
			width = atof(tokens[4].c_str());
			height = atof(tokens[5].c_str());
		}
	if (object_type == ObjType::QUESTIONBRICK)
	{
		ItemType = atof(tokens[4].c_str());
	}
	ani_set_id = atoi(tokens[3].c_str());
	if (object_type == ObjType::BRICK)
	{
		ItemType = atof(tokens[4].c_str());
	}
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
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(Level); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(ItemType,x,y); break;
	case OBJECT_TYPE_UNBREAK_BRICK: { obj = new CBrick(0,x,y); obj->ObjType = ObjType::UNBREAKABLEBRICK; break; }
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(Level); break;
	case OBJECT_TYPE_GROUND: obj = new Ground(width,height); break;
	case OBJECT_TYPE_WARPPIPE: obj = new WarpPipe(width, height); break;
	case OBJECT_TYPE_BLOCK: obj = new Block(width, height); break;
	case OBJECT_TYPE_QUESTIONBRICK: obj = new QuestionBrick(ItemType); break;
	case OBJECT_TYPE_FIREPIRANHAPLANT: obj = new FirePiranhaPlant(x,y); break;
	case OBJECT_TYPE_RED_KOOPAS: obj = new RedKoopas(); break;
	case OBJECT_TYPE_BUTTON: obj = new Button(x, y); break;
	case OBJECT_TYPE_BIGCOIN: {obj = new BigCoin(x, y); break; }
	case OBJECT_TYPE_PIRANHAPLANT:obj = new PiranhaPlant(x, y); break;
	/*case OBJECT_TYPE_ITEM: objects.push_back(item); break;*/
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	case OBJECT_TYPE_PORTAL_LASTSCENE:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		obj->ObjType = ObjType::LSPORTAL;
		break;
	}
	default:
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
	if (object_type == OBJECT_TYPE_BRICK)
	{
		Cbricks.push_back(obj);
	}
	if (object_type == OBJECT_TYPE_BUTTON)
	{
		Button* button = dynamic_cast<Button*>(obj);
		button->Bricks = Cbricks;
	}
	
	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);
	int Top = int(t / 115);
	int Left = int(l / 150);
	int Right = ceil(r / 150);
	int Bottom = ceil(b / 115);
	grid->InsertObj(obj, Left, Top, Right, Bottom);
 }

void CPlayScene::_ParseSection_MAP(string line)
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
	map = new Map(ID, FilePath_data.c_str(), Map_width, Map_height, Num_row_read, Num_col_read, Tile_width, Tile_height);
	if (A == 1)
	{
		map->IsWorldMap = true;
	}
}
void CPlayScene::_ParseSection_HUD(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation set must at least id and one animation id

	 int SpriteID = atof(tokens[2].c_str());
	 int SpriteStack = atof(tokens[3].c_str());
	 int SpritePower = atof(tokens[4].c_str());
	 float HUDX = atof(tokens[0].c_str());
	 float HUDY = atof(tokens[1].c_str());
	 Hud::GetInstance()->SpriteHUD= SpriteID;
	 Hud::GetInstance()->SpriteStack=SpriteStack;
	 Hud::GetInstance()->HUDx = HUDX;
	 Hud::GetInstance()->HUDy = HUDY;
	 Hud::GetInstance()->SpritePower= SpritePower;
}
void CPlayScene::_ParseSection_HUD_TIME(string line)
{
	vector<string> tokens = split(line);

	 // skip invalid lines - an animation set must at least id and one animation id

	float x = atof(tokens[10].c_str());
	float y = atof(tokens[11].c_str());
	int arr[10];
	for (int i = 0; i < 10; i++)
	{
		arr[i] = atof(tokens[i].c_str());
	}
	Hud::GetInstance()->hudTime = new HUD_Time(x, y, arr);
}
void CPlayScene::Load()
{
	Camera::GetInstance()->cam_y = 240;
	Camera::GetInstance()->cam_x = 0;
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
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[HUD]") {
			section = SCENE_SECTION_HUD; continue;
		}
		if (line == "[HUD_TIME]") {
			section = SCENE_SECTION_HUD_TIME; continue;
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
			case SCENE_SECTION_HUD: _ParseSection_HUD(line); break;
			case SCENE_SECTION_HUD_TIME: _ParseSection_HUD_TIME(line); break;

		}
	}
	f.close();
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	
	Hud::GetInstance()->Update(dt);
	grid->GetListObj(coNotMoveObjects, coMovingObjects,coObjects);
	for (size_t i = 0; i < coMovingObjects.size(); i++)
	{
		if (coMovingObjects[i]->GetHealth() != 0 && coMovingObjects[i]->ObjType != ObjType::MARIO)
		{
			if (coMovingObjects[i]->ObjType == ObjType::KOOPAS)
			{
				CKoopas* KP = dynamic_cast<CKoopas*>(coMovingObjects[i]);
				if (KP->IsAttack)
					coMovingObjects[i]->Update(dt, &coObjects);
				else
					coMovingObjects[i]->Update(dt, &coNotMoveObjects);

			}
			if (coMovingObjects[i]->ObjType == ObjType::REDKOOPAS)
			{
				CKoopas* KP = dynamic_cast<CKoopas*>(coMovingObjects[i]);
				if (KP->IsAttack)
					coMovingObjects[i]->Update(dt, &coObjects);
				else
					coMovingObjects[i]->Update(dt, &coNotMoveObjects);
			}
			else
			{
				if (coMovingObjects[i]->ObjType == ObjType::FIREPIRANHAPLANT)
				{
					coMovingObjects[i]->GetEnemyPos(player->x, player->y);
					coMovingObjects[i]->Update(dt, &coNotMoveObjects);
				}
				else
					coMovingObjects[i]->Update(dt, &coNotMoveObjects);
			}
			coMovingObjects[i]->IsInList = false;
		}
	}
	player->Update(dt, &coObjects);
	for (size_t i = 0; i < coNotMoveObjects.size(); i++)
	{
		if (coNotMoveObjects[i]->ObjType == ObjType::BRICK && coNotMoveObjects[i]->GetHealth() == 2)
		{
			CBrick* brick = dynamic_cast<CBrick*>(coNotMoveObjects[i]);
			if (brick->IsCollision)
			{
				Button* btn = new Button(brick->x, brick->y - 16);
				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set = animation_sets->Get(11);
				btn->SetAnimationSet(ani_set);
				btn->Bricks = Cbricks;
				objects.push_back(btn);
				brick->SubHealth();
			}
		}
		if (coNotMoveObjects[i]->ObjType == ObjType::QUESTIONBRICK && coNotMoveObjects[i]->GetHealth() == 2)
		{
			QuestionBrick* qb = dynamic_cast<QuestionBrick*>(coNotMoveObjects[i]);
			if (qb->ItemType == UNKNOW_ITEM)
			{
				if (player->level == MARIO_LEVEL_SMALL)
				{
					MushRoom* mushroom = new MushRoom(coNotMoveObjects[i]->x, coNotMoveObjects[i]->y);
					mushroom->CaclVx(player->x);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(MUSHROOM_ANISET_ID);
					mushroom->SetAnimationSet(ani_set);
					qb->SubHealth();
					objects.push_back(mushroom);
				}
				else
				{
					Leaf* leaf = new Leaf(coNotMoveObjects[i]->x, coNotMoveObjects[i]->y);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(LEAF_ANISET_ID);
					leaf->SetAnimationSet(ani_set);
					qb->SubHealth();
					objects.push_back(leaf);
				}
			}
			else if (qb->ItemType == ItemType::COIN)
			{
				DebugOut(L"Fuck\n");
				Coin* coin = new Coin(coNotMoveObjects[i]->x, coNotMoveObjects[i]->y);
				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set = animation_sets->Get(10);
				coin->SetAnimationSet(ani_set);
				qb->SubHealth();
				objects.push_back(coin);
			}
		}
		if (coNotMoveObjects[i]->Health != 0)
			coNotMoveObjects[i]->Update(dt, &coObjects);
		coNotMoveObjects[i]->IsInList = false;
	}
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	

	if (player->y <= 270)
	{
		if (player->IsOnPlatForm == false) {
			Camera::GetInstance()->Mariovy = player->vy;
			Camera::GetInstance()->IsFollowingMario = true;
		}
		else
		{
			Camera::GetInstance()->IsFollowingMario = false;
		}
	}
	Camera::GetInstance()->cam_x = player->x;
	Camera::GetInstance()->Update(dt);
	CGame *game = CGame::GetInstance();
	Camera::GetInstance()->cam_x -= game->GetScreenWidth() / 2;
	if (Camera::GetInstance()->cam_x < 0)
		Camera::GetInstance()->cam_x = 0;
	if (Camera::GetInstance()->cam_x + SCREEN_WIDTH > MAP_MAX_WIDTH)
		Camera::GetInstance()->cam_x = MAP_MAX_WIDTH - SCREEN_WIDTH;


	Hud::GetInstance()->MarioStack = player->CounterSpeed;
	Hud::GetInstance()->MarioMoney = player->Money;

	if (player->IsSwitchScene)
	{
		CGame::GetInstance()->SwitchScene(player->NextSceneID);
	}
	grid->UpdateGrid(objects);

}

void CPlayScene::Render()
{
	map->Draw();
	for (int i = 0; i < coNotMoveObjects.size(); i++)
	{
		if (coNotMoveObjects[i]->Health != 0 && coNotMoveObjects[i]->ObjType == ObjType::WARPPIPE)
			coNotMoveObjects[i]->Render();
	}
	objects[0]->Render();
	for (int i = 0; i < coMovingObjects.size(); i++)
	{
		if (coMovingObjects[i]->Health != 0 && coMovingObjects[i]->ObjType != ObjType::MARIO)
			coMovingObjects[i]->Render();
	}
	for (int i = 0; i < coNotMoveObjects.size(); i++)
	{
		if (coNotMoveObjects[i]->Health != 0 && coNotMoveObjects[i]->ObjType != ObjType::WARPPIPE)
			coNotMoveObjects[i]->Render();
	}
	Hud::GetInstance()->Render(player->score->TotalScore);
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	Cbricks.clear();
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
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->y -= 12;
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->y -= 12;
		mario->SetLevel(MARIO_LEVEL_RACOON);
		break;
	case DIK_4:
		mario->y -= 12;
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		if (mario->level == MARIO_LEVEL_RACOON)
		{
			mario->SetState(MARIO_STATE_SLOWFALLING);
			mario->SetState(MARIO_STATE_FLY);
		}
		break;
	case DIK_A:
		if (mario->level >= MARIO_LEVEL_RACOON)
			mario->SetState(MARIO_STATE_ATTACK);
		break;
	case DIK_DOWN:
		mario->GoHiddenMap = true;
		break;
	case DIK_H:
		mario->SetState(MARIO_STATE_GO_HIDDENMAP);
		break;
	case DIK_J:
		mario->SetPosition(2256, 80);
		Camera::GetInstance()->cam_x = mario->x / 2;
		Camera::GetInstance()->cam_y = mario->y / 2;
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_A:
		mario->StopRunning();
		mario->SetState(MARIO_STATE_DROPKOOPAS);
		break;
	case DIK_S:
		mario->IsSlowFalling = false; 
		if (mario->vy < 0)
			mario->vy = 0;
		break;
	case DIK_DOWN:
		mario->GoHiddenMap = false;
		break;
	case DIK_UP:
		mario->GoOutHiddenMap = false;
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (game->IsKeyDown(DIK_UP))
	{
		mario->GoOutHiddenMap = true;
	}
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