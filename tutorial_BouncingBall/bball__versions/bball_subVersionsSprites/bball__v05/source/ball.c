#include <nds.h>
#include "ball.h"


#define  c_radius (8<<8)
#define c_diam 16   // the radius of the ball in *.8 fixed point
                    // the diameter of the ball (integer)

void ballRender( ball* b, int camera_x, int camera_y )
{
 u16* sprite = OAM + b->sprite_index * 4; // Cada entrà de la taula de OAM ocupa 4 u16
	
 int x, y;
 x = ((b->x - c_radius*2) >> 8) - camera_x;
 y = ((b->y - c_radius*2) >> 8) - camera_y;
	
 if( x <= -16 || y <= -16 || x >= 256 || y >= 192 )
 {
	//sprites fuera de rango. Deshabilitar el sprite
	//sprite[0] = ATTR0_DISABLED;
        oamSetHidden( &oamMain, b->sprite_index, true);
	return;
 }
	
 sprite[0] = (y & 255) | ATTR0_ROTSCALE_DOUBLE;
 sprite[1] = (x & 511) | ATTR1_SIZE_16 | ATTR1_ROTDATA( b->sprite_affine_index );
 sprite[2] = 0;

 u16* affine;
 affine = OAM + b->sprite_affine_index * 16 + 3;
 affine[4] = 0;
 affine[8] = 0;

 int pa = (b->height * (65536/c_diam)) >> 16;
 int pd = 65536 / pa;
 affine[0] = pa;
 affine[12] = pd;


}


void ballRender_senseSquishing( ball* b, int camera_x, int camera_y )
{
 u16* sprite = OAM + b->sprite_index * 4; // Cada entrà de la taula de OAM ocupa 4 u16
	
int x, y;
x = ((b->x - c_radius) >> 8) - camera_x;
y = ((b->y - c_radius) >> 8) - camera_y;
	
if( x <= -16 || y <= -16 || x >= 256 || y >= 192 )
{
	//sprites fuera de rango. Deshabilitar el sprite
	//sprite[0] = ATTR0_DISABLED;
        oamSetHidden( &oamMain, b->sprite_index, true);
	return;
}
	
sprite[0] = y & 255;
sprite[1] = (x & 511) | ATTR1_SIZE_16;
sprite[2] = 0;
/* si tinguera acces a sprites --> canviar la interficie
sprites[b->sprite_index].attribute[0] = y & 255;
sprites[b->sprite_index].attribute[1] = (x & 511) | ATTR1_SIZE_16;
sprites[b->sprite_index].attribute[2] = 0;
*/
}



#define c_gravity 80            // gravity constant (add to vertical velocity) (*.8 fixed)
#define c_air_friction 1        //  friction in the air... multiply X velocity by (256-f)/256
#define c_ground_friction 30    //  friction when the ball hits the ground, multiply X by (256-f)/256
#define c_platform_level ((192-48) << 8)    //  the level of the brick platform in *.8 fixed point
#define c_bounce_damper 20      //  the amount of Y velocity that is absorbed when you hit the ground

#define min_height  (1200)   // the minimum height of the ball (when it gets squished) (*.8)
#define min_yvel (1200) // the minimum Y velocity (*.8)
#define max_xvel (1000<<4)  // the maximum X velocity (*.12)
//-----------------------------------------------------------------
// clamp integer to range
//-----------------------------------------------------------------
static inline int clampint( int value, int low, int high )
//-----------------------------------------------------------------
{
    if( value < low ) value = low;
    if( value > high) value = high;
    return value;
}

//-----------------------------------------------------------------
// update ball object (call once per frame)
//-----------------------------------------------------------------
void ballUpdate( ball* b ) // Físicas
{
 // add X velocity to X position xvel is 20.12 while x is 24.8
 b->x += (b->xvel>>4);

 //Gravetat: 
 // apply air friction to X velocity
 b->xvel = (b->xvel * (256-c_air_friction)) >> 8;
 
 // clamp X velocity to the limits
 b->xvel = clampint( b->xvel, -max_xvel, max_xvel );
 
 // add gravity to Y velocity
 b->yvel += c_gravity;
 // add Y velocity to Y position
 b->y += (b->yvel);

 // Bounce on the platform
 if( b->y + c_radius >= c_platform_level )
 {
    // apply ground friction to X velocity
    b->xvel = (b->xvel * (256-c_ground_friction)) >> 8;
    
    // check if the ball has been squished to minimum height
    if( b->y > c_platform_level - min_height )    {
     // mount Y on platform
     b->y = c_platform_level - min_height;
     
     // negate Y velocity, also apply the bounce damper
     b->yvel = -(b->yvel * (256-c_bounce_damper)) >> 8;
     
     // clamp Y to mininum velocity (minimum after bouncing, so the ball does not settle)
     if( b->yvel > -min_yvel ) b->yvel = -min_yvel;
    }
    
    // calculate the height
    b->height = (c_platform_level - b->y) * 2;
 }
 else {
    b->height = c_diam << 8;
 }
} // Fi ballUpdate

void ballUpdate__senseSquishing( ball* b ) // Físicas
{
 // add X velocity to X position xvel is 20.12 while x is 24.8
 b->x += (b->xvel>>4);

 //Gravetat: 
 // apply air friction to X velocity
 b->xvel = (b->xvel * (256-c_air_friction)) >> 8;
 
 // clamp X velocity to the limits
 b->xvel = clampint( b->xvel, -max_xvel, max_xvel );
 
 // add gravity to Y velocity
 b->yvel += c_gravity;
 // add Y velocity to Y position
 b->y += (b->yvel);

 // Bounce on the platform
 if( b->y + c_radius >= c_platform_level ) {
    // apply ground friction to X velocity
    b->xvel = (b->xvel * (256-c_ground_friction)) >> 8;
    
    // mount Y on platform
    b->y = c_platform_level - c_radius;
    
    // negate Y velocity, also apply the bounce damper
    b->yvel = -(b->yvel * (256-c_bounce_damper)) >> 8;
    
    // clamp Y to mininum velocity (minimum after bouncing, so the ball does not settle)
    if( b->yvel > -min_yvel ) b->yvel = -min_yvel;
 }

} // fi de ballUpdate__senseSquishing

