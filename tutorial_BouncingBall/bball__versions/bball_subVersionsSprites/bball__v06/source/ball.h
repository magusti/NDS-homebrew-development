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
	
	int height;	//ancho del balón
} ball;

void ballUpdate( ball* b );
void ballRender( ball* b, int camera_x, int camera_y );


#define  c_radius (8<<8)
#define c_diam 16   // the radius of the ball in *.8 fixed point
                    // the diameter of the ball (integer)

#endif
