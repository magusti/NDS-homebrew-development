#include <nds.h>
#include "ball.h"

#define c_gravity			80				//constante de gravedad (añadir velocidad vertical) (*.8 fixed)

#define c_air_friction		1				//friccion con el aire... Multiplicador velocidad X por (256-f)/256
#define c_ground_friction	30				//friccion cuando el balon choca con el suelo, multiplicador X por (256-f)/256
#define c_platform_level	((192-48) << 8)	//El nivel de la plataforma de ladrillos en *.8 fixed point
#define c_bounce_damper		20				//la cantidad de velocidad Y que es absorvida cuando golpeas el suelo

#define c_radius			(8<<8)			//El radio del balon en *.8 fixed point
#define c_diam				16				//el diametro del balon (entero)

#define min_heigth			(1200)			//la minima anchura del balon (cuando es aplastado) (*.8)

#define min_yvel			(1200)			//la minima velocidad Y (*.8)
#define max_xvel			(1200<<4)		//la maxima velocidad X (*.12)

//fijar en rango un numero entero
static inline int clampint(int value, int low, int high)
{
	if( value < low ) value = low;
	if( value > high ) value = high;
	return value;
}

//Actualizar objeto balon (una llamada por frame)
void ballUpdate( ball* b )
{
	//Añadir X velocidad a la posicion X
	b->x += (b->xvel>>4);
	
	//aplicar friccion del aire a la velocidad X
	b->xvel = (b->xvel * (256-c_air_friction)) >> 8;
	
	//fijar velocidad Xen los limites
	b->xvel = clampint( b->xvel, -max_xvel, max_xvel );
}

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