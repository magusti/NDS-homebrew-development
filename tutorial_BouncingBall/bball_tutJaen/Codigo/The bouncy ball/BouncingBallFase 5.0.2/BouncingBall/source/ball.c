#include <nds.h>
#include "ball.h"

/**Añadido al original**/
#define c_both_level	((192 - 8) << 8)//límite inferior
#define c_top_level	( (0 - 8 )<< 8)		//límite superior
#define c_izq_level ((0 - 8 )<< 8)		//límite izquierdo
#define c_der_level ((256 - 8 ) << 8)	//límite derecho

#define c_diam				32				//el diametro del balon (entero)
/**Añadido al original**/

#define c_radius (8 << 8)


//#define min_height			(1200)			//la minima anchura del balon (cuando es aplastado) (*.8)


void ballUpdate( ball* b )
{
	/**Añadido al original**/
	//Control del límite de la pantalla
	if(b->y + c_radius >= c_both_level ) b->y = c_both_level - c_radius;
	if(b->y - c_radius <= c_top_level ) b->y = c_top_level + c_radius;
	if(b->x - c_radius <= c_izq_level ) b->x = c_izq_level + c_radius;
	if(b->x + c_radius >= c_der_level ) b->x = c_der_level - c_radius;
	
	//Control del tamaño de la bola
	if(b->height <= (16 << 8)) b->height = (16 << 8);
	if(b->height >= (64 << 8)) b->height = (64 << 8);
	/**Añadido al original**/
}
void ballRender( ball* b, int camera_x, int camera_y)
{
	u16* sprite = OAM + b->sprite_index * 4;
	
	/**Añadido al original**/
	u16* affine = OAM + b->sprite_affine_index * 16 + 3;
	/**Añadido al original**/
	
	//u16* affineS = OAM + b->sprite_affine_indexS * 16 + 3;
	//u16* affineR = OAM + b->sprite_affine_indexR * 16 + 3;
	
	int x, y;
	x = ((b->x - c_radius) >> 8) - camera_x;
	y = ((b->y - c_radius) >> 8) - camera_y;
	
	/**Añadido al original**/
	int pa, pb, pc, pd;
	
	if(b->scale){
	pa = (b->height * (65536/c_diam)) >> 16;
	pd = (b->height * (65536/c_diam)) >> 16;
	pb = 0;
	pc = 0;
	}
	else
	{
	pa = cosLerp(b->alpha) >> 4;
	pd = cosLerp(b->alpha) >> 4;
	pb = sinLerp(b->alpha) >> 4;
	pc = -sinLerp(b->alpha) >> 4;
	}
	/**Añadido al original**/
	
	sprite[0] = (y & 255) | ATTR0_ROTSCALE_DOUBLE;
	sprite[1] = (x & 511) | ATTR1_SIZE_16 | ATTR1_ROTDATA( b->sprite_affine_index );
	//sprite[1] = (x & 511) | ATTR1_SIZE_16 | ATTR1_ROTDATA( b->sprite_affine_indexS ) | ATTR1_ROTDATA( b->sprite_affine_indexR );
	sprite[2] = 0;
	
	/**Añadido al original**/
	affine[0] = pa;
	affine[4] = pb;
	affine[8] = pc;
	affine[12] = pd;
	/**Añadido al original**/
	
	/*if(b->sprite_affine_index == 0)
	{
		affine[0] = paScale;
		affine[4] = pbScale;
		affine[8] = pcScale;
		affine[12] = pdScale;
	}
	else
	{
		affine[0] = paRot;
		affine[4] = pbRot;
		affine[8] = pcRot;
		affine[12] = pdRot;
	}*/
	
	
}

