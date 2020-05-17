#include <nds.h>
#include <stdio.h>

#include <maxmod9.h>
#include <soundbank.h>
#include <soundbank_bin.h>


// Referencias a los gráficos y sonido que se generan en "build" por el Makefile "automágicamente"
#include <gfx_ball.h>
#include <gfx_brick.h>
#include <gfx_gradient.h>
#include "ball.h"

ball g_ball;
void resetBall( void );


//void setupGraphics();
void setupGraphics( int *bg0, int *bg1 );
//void updateGraphics();
void updateGraphics( int bg0, int bg1 );
void processLogic( void );

// 
#include "soundbank.h"

void inicializarSonidos( void );
void liberarRecursosSonidos( void );


int main( void )
{
   //uint8 laFetxa;
   int bg0, bg1;
  
   // Initialize the console to a default state for prototyping. This function sets the console to use sub display, VRAM_C, and BG0 and enables MODE_0_2D on the sub display. It is intended for use in prototyping applications which need print ability and not actual game use. Print functionality can be utilized with just this call. 
    consoleDemoInit();
    
    //setupGraphics();
    setupGraphics( &bg0, &bg1 );
    resetBall();
    
    inicializarSonidos();

    
    while(1)
    {
 	 //Interacción con el usuario y calcular posiciones, velocidades, etc de  objetos
  	 //update_logic --> updateLogic(); --> processLogic
	 processLogic();
        
         printf("How to make a Bouncing Ball Game\n"); 

	 //Espera vblank (seguro modificar graficos)
	 swiWaitForVBlank();		
	 
	 //Actualizacion objetos del juego 
	 //updateGraphics();
         updateGraphics( bg0, bg1 );
     
    }
    
    liberarRecursosSonidos();
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


// Teselas (tiles) para los fondos
#define tile_empty		0 //tile 0 = empty
#define tile_brick		1 //tile 1 = brick
#define tile_gradient           2 //tile 2 = gradient

//Paletas de las teselas de los fondos
#define pal_bricks              0 //paleta brick (entrada 0->15)
#define pal_gradient            1 //paleta gradient (entrada 16->31)

#define backdrop_colour	        RGB8( 190, 255, 255 )

// Sprite
#define tiles_ball              0 // ball tiles (16x16 tile 0->3)
#define sprites ((SpriteEntry*)OAM)
#define pal_ball 0 // ball palette (entry 0->15)


void setupGraphics( int *bg0, int *bg1 )
{
 int n, x, y; 
 // Fondos, niveles o capas de la escena
 u16 *bg0map, *bg1map;

 // put the main screen on the bottom lcd
 lcdMainOnTop(); // ...Bottom();
        
 // 1.1
 vramSetBankE( VRAM_E_MAIN_BG );
 vramSetBankF( VRAM_F_MAIN_SPRITE );
      
  
 *bg0 = bgInit(0, BgType_Text4bpp, BgSize_T_256x256, 1, 0); 
 *bg1 = bgInit(1, BgType_Text4bpp, BgSize_T_256x256, 2, 1); 

 // 1.2: generar el primer banco de tile por borrado a cero
 for( n = 0; n < 16; n++ )     {       BG_GFX[n] = 0;    }
 //Cargando los graficos para las teselas de las capas
 dmaCopy( gfx_brickTiles, (BG_GFX + (tile_brick * 16)), gfx_brickTilesLen );      
 dmaCopy( gfx_gradientTiles, (BG_GFX + (tile_gradient * 16)),           gfx_gradientTilesLen ); 
 
  //Cargando las paletas para las teselas de las capas
  dmaCopy( gfx_brickPal, BG_PALETTE, gfx_brickPalLen );
  dmaCopy( gfx_gradientPal, BG_PALETTE+(pal_gradient*16), gfx_gradientPalLen );
    
  //Asignar Color de fondo
  BG_PALETTE[0] = backdrop_colour;
 
  bg0map = bgGetMapPtr( *bg0 );
  bg1map = bgGetMapPtr( *bg1 );

  //Construir los mapas
  for( n = 0; n < 1024; n++ ) bg0map[n] = 0;
  
  for( x = 0; x < 32; x++ )    {
     for( y = 18; y < 24; y++ ) 
     {
         // magical formula to calculate if the tile needs to be flipped.
         int hflip = (x & 1) ^ (y & 1);
            
         // set the tilemap entry
         bg0map[x + y * 32] = tile_brick | (hflip << 10) | (pal_bricks << 12);
     }
  }
    
  for( n = 0; n < 1024; n++ ) bg1map[n] = 0;
  for( x = 0; x < 32; x++ ) {
     for( y = 0; y < 8; y++ )
     {
           int tile = tile_gradient + y;
           bg1map[ x + y * 32 ] = tile | (pal_gradient << 12);
     }
  }
    
  // Combinar el gradiente (BG1) con el "backdrop".
  REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG1 | BLEND_DST_BACKDROP;
  //De l'original es veu molt brillant, aixina que el baixa 
  //REG_BLDALPHA = (16<<8) + (4); //SRC_BG1*16 + BACKDROP*4
  REG_BLDALPHA = (16<<8) + (16); //SRC_BG1*16 + BACKDROP*4
  //
  // void bgClearControlBits( 	int  	id, 	u16  	bits 	) ; ?????	
  //

  // Inicializar el modo de vídeo
  videoSetMode( MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE );
  // Inicializar el uso de sprites
  oamInit(&oamMain, SpriteMapping_1D_32, false); //!extPalette

  // Cargar los sprites
  dmaCopy( gfx_ballTiles, SPRITE_GFX, gfx_ballTilesLen );
  dmaCopy( gfx_ballPal, SPRITE_PALETTE, gfx_ballPalLen );

} // Fi de setupGraphics



int g_camera_x;
int g_camera_y;


// Menejar pilota i càmera
void updateGraphics( int bg0, int bg1 )
{
 //oamUpdate( &oamMain );
 ballRender( &g_ball, 0, 0 );    

 //REG_BG0HOFS = g_camera_x >> 8;
 //bgScroll( bg0, g_camera_x, 0 );
 bgSetScroll( bg0, g_camera_x, 0 );
 bgUpdate(); // Must be called once per frame to update scroll/scale/and rotation of backgrounds. 
  
 
}

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

 




void inicializarSonidos( void) {
    // 
    //Inicialiamos Maxmod
    mmInitDefaultMem((mm_addr)soundbank_bin);
    
    //Cargamos modulo de musica
    mmLoad(MOD_BSMUSIC);
    mmSetModuleVolume( 512 ); // New volume level. Ranges from 0 (silent) to 1024 (normal).
    
    //Cargamos sonido FX
    mmLoadEffect( SFX_BOING );
    mmEffectVolume( SFX_BOING, 128 ); //Ranges from 0 (silent) to 255 (normal).
         
    //Comenzamos la musica de fondo
    mmStart(MOD_BSMUSIC, MM_PLAY_LOOP);
} // Fi de inicializarSonidos


void liberarRecursosSonidos( void ) {
    mmStop();
    mmUnload(MOD_BSMUSIC);
    mmUnloadEffect(SFX_BOING);
}// Fi de liberarRecursosSonidos

