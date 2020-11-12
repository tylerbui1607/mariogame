#include "FireBullet.h"
#include "Mario.h"
#define	FIREBULLET_GRAVITY		0.0007f

void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (GetHealth() != 0)
	{
		CGameObject::Update(dt);
		if (FireMario)
		{
			vy += GRAVITY * dt;
		}
			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;

			coEvents.clear();
			CalcPotentialCollisions(coObjects, coEvents);
			if (coEvents.size() == 0)
			{
				x += dx;
				y += dy;
			}
			else
			{
				float min_tx, min_ty, nx = 0, ny;
				float rdx = 0;
				float rdy = 0;
				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
				// Collision logic with Goombas
				for (UINT i = 0; i < coEventsResult.size(); i++)
				{
					LPCOLLISIONEVENT e = coEventsResult[i];
					if (e->obj->ObjType != ObjType::MARIO)
					{
						if (e->ny < 0)
						{
							vy = -0.15;
						}
						if (e->nx && e->obj->ObjType != ObjType::BLOCK)
						{
							SubHealth();
							vx = 0;
							vy = 0;
						}
						else 
						{
							x += dx;
						}
						y += min_ty * dy + ny * 0.5f;
					}
					else
					{
						CMario* p = dynamic_cast<CMario*>(e->obj);
						p->Calclevel();
						x += dx;
						y += dy;
					}
				}
			}
	}
}
