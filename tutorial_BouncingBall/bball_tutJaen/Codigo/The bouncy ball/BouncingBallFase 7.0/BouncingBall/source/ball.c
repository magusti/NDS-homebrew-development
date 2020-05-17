#include <nds.h>
#include "ball.h"

#define c_gravity			80				//constante de gravedad (añadir velocidad vertical) (*.8 fixed)

#define c_air_friction		1				//friccion con el aire... Multiplicador velocidad X por (256-f)/256
#define c_ground_friction	30				//friccion cuando el balon choca con el suelo, multiplicador X por (256-f)/256
#define c_platform_level	((192-48) << 8)	//El nivel de la plataforma de ladrillos en *.8 fixed point
#define c_bounce_damper		20				//la cantidad de velocidad Y que es absorvida cuando golpeas el suelo

#define c_radius			(8<<8)			//El radio del balon en *.8 fixed point
#define c_diam				16				//el diametro del balon (entero)

#define min_height			(1200)			//la minima anchura del balon (cuando es aplastado) (*.8)

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
	
	//añadir gravedad a la velocidad Y
	b->yvel += c_gravity;
	
	//añadir velocidad Y a la posicion Y
	b->y += (b->yvel);
	
	if(b->y + c_radius >= c_platform_level )
	{
		//aplicar friccion de suelo a la velocidad X
		//(si, esto puede hacerse multiples veces)
		b->xvel = (b->xvel * (256-c_ground_friction)) >> 8;
		
		//comprobar si el balon a sido aplastado a la altura mínima
		if(b->y > c_platform_level - min_height)
		{
			//montaje plataforma Y
			b->y = c_platform_level - min_height;
		
			//Negar velocidad Y, mientras aplicar amortiguador de rebote
			b->yvel = -(b->yvel * (256-c_bounce_damper)) >> 8;
		
			//fijar la minima velocidad Y (minimo despues de rebotar, para que el balon no se desestabilice
			if( b->yvel > -min_yvel )
			{
				b->yvel = -min_yvel;
			}
		}
		
		//Calcular la altura
		b->height = (c_platform_level - b->y) * 2;
	}
	else
	{
		b->height = c_diam << 8;
	}
}

void ballRender( ball* b, int camera_x, int camera_y )
{
	u16* sprite = OAM + b->sprite_index * 4;
	u16* affine = OAM + b->sprite_affine_index * 16 + 3;
	
	int x, y;
	x = ((b->x - c_radius * 2) >> 8) - camera_x;
	y = ((b->y - c_radius * 2) >> 8) - camera_y;
	
	int pa = (b->height * (65536/c_diam)) >> 16;
	int pd = 65536 / pa;
	
	if( x <= -16 || y <= -16 || x >= 256 || y >= 192 )
	{
		//sprites fuera de rango
		//Deshabilitar el sprite
		sprite[0] = ATTR0_DISABLED;
		return;
	}
	
	sprite[0] = (y & 255) | ATTR0_ROTSCALE_DOUBLE;
	sprite[1] = (x & 511) | ATTR1_SIZE_16 | ATTR1_ROTDATA( b->sprite_affine_index );
	sprite[2] = 0;
	
	affine[0] = pa;
	affine[4] = 0;
	affine[8] = 0;
	affine[12] = pd;
	
	
	
}