#include <nds.h>
#include <stdio.h>

// Referencias a los gráficos y sonido que se generan en "build" por el Makefile "automágicamente"
#include <gfx_brick.h>
#include <gfx_gradient.h>

//void setupGraphics();
void setupGraphics( int *bg0, int *bg1 );
//void updateGraphics();
void updateGraphics( int bg0, int bg1 );
void processLogic( void );


int main( void )
{
    int bg0, bg1;
      
    //setupGraphics();
    setupGraphics( &bg0, &bg1 );
    
    
    while(1)
    {
 	 //Interacción con el usuario y calcular posiciones, velocidades, etc de  objetos
  	 //A lo largo del tut. original cambia de 
         // update_logic --> updateLogic(); --> processLogic
	 processLogic();
        
         printf("How to make a Bouncing Ball Game\n"); 

	 //Espera vblank (seguro modificar graficos)
	 swiWaitForVBlank();		
	 
	 //Actualizacion objetos del juego 
	 //En el tut. orginal era updateGraphics();
         updateGraphics( bg0, bg1 );
     
    }
    
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
 
} // Fi de setupGraphics



void updateGraphics( int bg0, int bg1 )
{
 
}


void processLogic( void )
{
 
    
} // fi de processLogic

