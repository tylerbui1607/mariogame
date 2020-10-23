#include "FireBullet.h"

void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetHealth() != 0)
	{
		CGameObject::Update(dt);
		if (FireMario)
		{
			vy += 0.0007 * dt;
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
					else if (e->obj->ObjType == ObjType::BLOCK)
					{
						x += dx;
					}
					y += min_ty * dy + ny * 0.5f;
				}
			}
		}
		else
		{
			x += dx;
			y += dy;
		}
	}
}
