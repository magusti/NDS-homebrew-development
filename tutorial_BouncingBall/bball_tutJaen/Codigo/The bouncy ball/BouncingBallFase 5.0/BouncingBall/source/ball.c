#include <nds.h>
#include "ball.h"

#define c_radius (8<<8)

void ballUpdate( ball* b ){}
void ballRender( ball* b, int camera_x, int camera_y )
{
	u16* sprite = OAM + b->sprite_index * 4;
	
	int x, y;
	x = ((b->x - c_radius) >> 8) - camera_x;
	y = ((b->y - c_radius) >> 8) - camera_y;
	
	if( x <= -16 || y <= -16 || x >= 256 || y >= 192 )
	{
		//sprites fuera de rango
		//Deshabilitar el sprite
		sprite[0] = ATTR0_DISABLED;
		return;
	}
	
	sprite[0] = y & 255;
	sprite[1] = (x & 511) | ATTR1_SIZE_16;
	sprite[2] = 0;
}