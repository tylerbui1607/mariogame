#include "MarioWM.h"
#include "Portal.h"
#include"Game.h"
#include "Utils.h"
#define	Mario_Animation	0
void MarioWM::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->ObjType == ObjType::PORTAL)
		{
			if (CheckAABB(coObjects->at(i)))
			{
				CPortal* p = dynamic_cast<CPortal*>(coObjects->at(i));
				if (IdPortal != p->ID)
				{
					if (vx != 0)
					{
						if (vx >= 0)
						{
							if (x - p->x >= 2)
							{
								vx = 0;
								IdPortal = p->ID;
							}
						}
						else
						{
							if (x - p->x <= 2)
							{
								vx = 0;
								IdPortal = p->ID;
							}
						}
					}
					else if (vy != 0)
					{
							if (vy >= 0)
							{
								if (y - p->y >= 2)
								{
									vy = 0;
									IdPortal = p->ID;
								}
							}
							else
							{
								if (y - p->y <= 2)
								{
									vy = 0;
									IdPortal = p->ID;
								}
							}
					}

					CanGoLeft = p->Check[0];
					CanGoRight = p->Check[1];
					CanGoUp = p->Check[2];
					CanGoDown = p->Check[3];
				}
					if (Choosen)
					{
						CGame::GetInstance()->SwitchScene(p->GetSceneId());
					}
					
			}
		}
	}
	CGameObject::Update(dt);

	x += dx;
	y += dy;
}

void MarioWM::Render()
{
	animation_set->at(Mario_Animation)->RenderWM(x,y);
}

void MarioWM::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_WALK_LEFT:
		if (vx == 0 && vy == 0 && CanGoLeft == 1)
		vx = -0.1;
		break;
	case MARIO_STATE_WALK_RIGHT:
		if (vx == 0 && vy == 0 && CanGoRight == 1)
		vx = 0.1;
		break;
	case MARIO_STATE_WALK_UP:
		if (vx == 0 && vy == 0 && CanGoUp == 1)
		vy = -0.1;
		break;
	case MARIO_STATE_WALK_DOWN:
		if (vx == 0 && vy == 0 && CanGoDown == 1)
		vy = 0.1;
		break;
	}
}
