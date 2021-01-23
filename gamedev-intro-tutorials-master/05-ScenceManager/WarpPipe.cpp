#include "WarpPipe.h"

void WarpPipe::Render()
{
	int drawy = y + TileSize;
	for (int i = 0; i < (Height / TileSize)*2; i++)
	{
		if (i < 2)
			animation_set->at(i)->Render(i * TileSize + x, y);
		else if (i % 2 == 0)
		{
			
			animation_set->at(Ani_Body_Right)->Render(x,drawy);
		}
		else
		{
			animation_set->at(Ani_Body_Left)->Render(x + TileSize, drawy);
			drawy += TileSize;
		}
	}
	
}
