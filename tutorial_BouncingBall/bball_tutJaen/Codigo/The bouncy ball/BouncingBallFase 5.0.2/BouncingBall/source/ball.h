//ball.h

#ifndef BALL_H
#define BALL_H

typedef struct t_ball
{
	int x;		//24.8 punto fijo
	int y;		//24.8 punto fijo
	int xvel;	//20.12 punto fijo
	int yvel;	//24.8 punto fijo
	
	u8 sprite_index;		//entrada OAM (0->127)
	u8 sprite_affine_index;	//entrada OAM affine (0->31)
	//u8 sprite_affine_indexS;
	//u8 sprite_affine_indexR;
	
	int height;	//ancho del bola
	
	/**Añadido al original**/
	int alpha; //Ángulo de la bola
	bool scale; //Mira si esta con rotacion o escalado: true:escalado, false: rotacion
	/**Añadido al original**/
	
} ball;

void ballUpdate( ball* b );
void ballRender( ball* b, int camera_x, int camera_y);

#endif