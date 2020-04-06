
#include <nds.h>
#include <stdio.h>

//Referenias Graficas que se generan en "build" por el Makefile "automágicamente"
#include <gfx_ball.h>
#include <gfx_brick.h>
#include <gfx_gradient.h>
//#include "soundbank.h"
#include "ball.h"

ball g_ball;
void setupInterrupts( void );
void resetBall( void );

void setupGraphics();
void updateLogic();	
void updateGraphics();

void processLogic( void );


int main( void )
{
    //setupInterrupts();
    setupGraphics();
    resetBall();
    
    while(1)
    {
		//Periodo de Renderizado
		//Actualizacion objetos del juego (Moverlos alrededor,                        
                       //calcular velocidades, etc) 
		//update_logic --> updateLogic(); --> processLogic
		processLogic();
                //printf("Escriu alguna cosa"); NO ESCRIU RES!

		//Espera para periodo vblank
		swiWaitForVBlank();
		
		//Periodo vblank: (seguro modificar graficos)
		//mover graficos alrededor
		updateGraphics();
    }
}

//-----------------------------------------------------------
// setup interrupt handler with vblank irq enabled
   // ~/docencia/2k19-2k20/docenciaEnXarxa/desarrollNDS/libnds-master/source/arm9/system/initSystem.c
   // Reset the DS registers to sensible defaults
   /*
    * void __attribute__((weak)) initSystem(void)
       irqInit();
       ... 
       irqEnable(IRQ_VBLANK);
   */
//-----------------------------------------------------------
void setupInterrupts( void )
//-----------------------------------------------------------
{
 // initialize interrupt handler
 irqInit();
 // enable vblank interrupt (required for swiWaitForVBlank!)
 irqEnable( IRQ_VBLANK );
}

//-----------------------------------------------------------
// reset ball attributes
//-----------------------------------------------------------
void resetBall( void )
//-----------------------------------------------------------
{
  // use sprite index 0 (0->127)
  g_ball.sprite_index = 0;

  // use affine matrix 0 (0->31)
  g_ball.sprite_affine_index = 0;

  // X = 128.0
  g_ball.x = 128 << 8;
  
  // Y = 64.0
  g_ball.y = 64 << 8;
  
  // start X velocity a bit to the right
  g_ball.xvel = 100 << 4;
  
  // reset Y velocity
  g_ball.yvel = 0;
}


//Entradas tile
// 1
#define tile_empty		0 //tile 0 = empty
#define tile_brick		1 //tile 1 = brick
#define tile_gradient	      2 //tile 2 = gradient

//macro para calcular memoria BG VRAM
//direccion con el indice de tile
#define tile2bgram(t) (BG_GFX + (t) *16)

//Paletas de entrada
#define pal_bricks	0 //paleta brick (entrada 0->15)
#define pal_gradient	1 //paleta gradient (entrada 16->31)

#define backdrop_colour	RGB8( 190, 255, 255 )
#define pal2bgram(p)	(BG_PALETTE + (p) * 16)


// 2
#define bg0map    ((u16*)BG_MAP_RAM(1))
#define bg1map    ((u16*)BG_MAP_RAM(2))


// Sprites
#define tiles_ball  0 // ball tiles (16x16 tile 0->3)
#define tile2objram(t) (SPRITE_GFX + (t) * 16)

/* Ja no fa falta:: En sprite.h:: SpriteEntry -->  attribute[3];
typedef struct t_spriteEntry
{
 u16 attr0;
 u16 attr1;
 u16 attr2;
 u16 affine_data;
} spriteEntry;
*/

//#define sprites ((spriteEntry*)OAM)
#define sprites ((SpriteEntry*)OAM)

#define pal2objram(p) (SPRITE_PALETTE + (p) * 16)
#define pal_ball 0 // ball palette (entry 0->15)


void setupGraphics( void )
{
 // put the main screen on the bottom lcd
 lcdMainOnBottom();
        
 // 1.1
 vramSetBankE( VRAM_E_MAIN_BG );
 vramSetBankF( VRAM_F_MAIN_SPRITE );
      
// 1.2
    //generar el primer banco de tile por borrado a cero:: NECESSARI???
  int n;
  /*
    for( n = 0; n < 16; n++ )
    {
       BG_GFX[n] = 0;
    }
*/
 //Copiando graficos
    dmaCopyHalfWords( 3, gfx_brickTiles, tile2bgram( tile_brick ), gfx_brickTilesLen );
    
    dmaCopyHalfWords( 3, gfx_gradientTiles, tile2bgram( tile_gradient ), gfx_gradientTilesLen );
    
    // 1.3
    //Paleta direccionada a la memoria de paleta
    dmaCopyHalfWords( 3, gfx_brickPal, pal2bgram( pal_bricks ), gfx_brickPalLen );
    dmaCopyHalfWords( 3, gfx_gradientPal, pal2bgram( pal_gradient ), gfx_gradientPalLen );
	
//Asignar Color de fondo
BG_PALETTE[0] = backdrop_colour;
//BG_PALETTE[1] = RGB8( 0, 255, 0 ); // --> taulells verds
//BG_PALETTE[1] = RGB8( 0, 0, 255 ); // --> taulells blaus
    // 1.4
// Actualitzat per 2.3
// videoSetMode( MODE_0_2D );
 

// 2.1

// libnds prefixes the register names with REG_BGxCNT
REG_BG0CNT = BG_MAP_BASE(1);
REG_BG1CNT = BG_MAP_BASE(2);
//int n;
for( n = 0; n < 1024; n++ ) bg0map[n] = 0;

// 2.2
int x, y;
for( x = 0; x < 32; x++ )
{
    for( y = 18; y < 24; y++ ) // ó for( y = 0; y < 6; y++ ) + després REG_BG0VOFS = 112;
    {
         // magical formula to calculate if the tile needs to be flipped.
         int hflip = (x & 1) ^ (y & 1);
            
         // set the tilemap entry
         bg0map[x + y * 32] = tile_brick | (hflip << 10) | (pal_bricks << 12);
    }
}
// 2.3
//videoSetMode( MODE_0_2D | DISPLAY_BG0_ACTIVE );

// 2.4 Si no el modifique o el deixe en '0' no es meneja
// REG_BG0VOFS = 112;

// 3.1
for( n = 0; n < 1024; n++ ) bg1map[n] = 0;

// 3.2
for( x = 0; x < 32; x++ )
{
     for( y = 0; y < 8; y++ )
     {
           int tile = tile_gradient + y;
           bg1map[ x + y * 32 ] = tile | (pal_gradient << 12);
     }
}
// Necessari abans d'incorporar un sprite
// videoSetMode( MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE );

// Combinar el gradiente (BG1) con el "backdrop".
REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG1 | BLEND_DST_BACKDROP;
//REG_BLDALPHA = (16) + (16<<8); De l'original es veu molt brillant, aixina que el baixa 
REG_BLDALPHA = (4) + (16<<8);


//
// Sprite
//
// Esta inicialització ara es:
// videoSetMode( MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D_LAYOUT );
videoSetMode( MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE );
oamInit(&oamMain, SpriteMapping_1D_32, false); //!extPalette

dmaCopyHalfWords( 3, gfx_ballTiles, tile2objram(tiles_ball), gfx_ballTilesLen );
dmaCopyHalfWords( 3, gfx_ballPal, pal2objram(pal_ball), gfx_ballPalLen );

/* Açò ho fa ja libnds/sprite.c en oamInit
//int n;
for( n = 0; n < 128; n++ )
    sprites[n].attr0 = ATTR0_DISABLED;
*/



} // Fi de setupGraphics



int g_camera_x;
int g_camera_y;

//-----------------------------------------------------------
// update graphical information (call during vblank)
//-----------------------------------------------------------
void updateGraphics__provar51Sprites( void )
//-----------------------------------------------------------
{
 // Ací deuria anar un 
 //oamUpdate( &oamMain );
/*
// code to test out the sprite engine
int n;
for( n = 0; n < 50; n++ )
{
// attribute0: set vertical position 0->screen_height-sprite_height,
// other default options will be okay (default == zeroed)
//sprites[n].attr0 = rand() % (192 - 16);
sprites[n].attribute[0] = rand() % (192 - 16);
//No! sprites[n].y = rand() % (192 - 16);

// attribute1: set horizontal position 0->screen_width-sprite_width
// also set 16x16 size mode
//sprites[n].attr1 = (rand() % (256 - 16)) + ATTR1_SIZE_16;
sprites[n].attribute[1] = (rand() % (256 - 16)) + ATTR1_SIZE_16;
// No! sprites[n].x = (rand() % (256 - 16)) + ATTR1_SIZE_16;

// attribute0: select tile number and palette number
//sprites[n].attr2 = tiles_ball + (pal_ball << 12);
//sprites[n].attr2 = tiles_ball + (pal_ball << 12);
//sprites[n].attribute[2] = tiles_ball + (pal_ball << 12);
sprites[n].gfxIndex = tiles_ball;
sprites[n].palette = (pal_ball << 12);
}

* sprite.h
typedef union SpriteEntry 	SpriteEntry
void 	oamSet (OamState *oam, int id, int x, int y, int priority, int palette_alpha, SpriteSize size, SpriteColorFormat format, const void *gfxOffset, int affineIndex, bool sizeDouble, bool hide, bool hflip, bool vflip, bool mosaic)
 	sets an oam entry to the supplied values More...

*/
 // update ball sprite, camera = 0, 0
ballRender( &g_ball, 0, 0 );    


REG_BG0HOFS = g_camera_x >> 8;
}

void updateGraphics_noMesLaPilota( void )
//-----------------------------------------------------------
{
 // Ací deuria anar un 
 //oamUpdate( &oamMain );
ballRender( &g_ball, 0, 0 );    
}

// Menejar pilota i càmera
void updateGraphics( void )
{
 // Ací deuria anar un 
 //oamUpdate( &oamMain );
ballRender( &g_ball, 0, 0 );    

REG_BG0HOFS = g_camera_x >> 8;
}


/*
 * 
void updateLogic()
{
    
}
*/
#define x_tweak (1<<2) // for user input  (Ho baixe de (2<<8)
#define y_tweak 50  // for user input: quan més gran més salta

void processInput( void )
{
  scanKeys();

 int keysh = keysHeld();
 // process user input
 if( keysh & KEY_UP ) { // check  if UP is pressed --> tweak y velocity of ball
  g_ball.yvel -= y_tweak;
 }
 
 if( keysh & KEY_DOWN ){ // check if DOWN is pressed --> tweak y velocity of ball
  g_ball.yvel += y_tweak;
 }

 if( keysh & KEY_LEFT ) {// check if LEFT is pressed --> tweak x velocity
  g_ball.xvel -= x_tweak;
 }

 if( keysh & KEY_RIGHT ) { // check if RIGHT is pressed --> tweak y velocity
  g_ball.xvel += x_tweak;
 }
 
 } // Fi de processInput


 void updateCamera( void ) {
 // cx = desired camera X (en format 24.8)
 int cx = ((g_ball.x)) - (128 << 8); 

 // dx = difference between desired and current position
 int dx;
 dx = cx - g_camera_x;

 // 10 is the minimum threshold
 if( dx > 10 || dx < -10 )  //10
   dx = (dx * 50) >> 1; // scale the value by some amount // 50 i 10

 // add the value to the camera X position
 g_camera_x += dx;

 // camera Y is always 0
 g_camera_y = 0;
 
 } // Fi de updateCamera

 
void processLogic( void )
{
 processInput();
 ballUpdate( &g_ball );

 /*
 "Whoops, the ball went out of the screen."
To fix this, we will add a 'camera' that will follow the ball around the screen. Make two more global variables.
*/
 updateCamera();
} // fi de processLogic

 


