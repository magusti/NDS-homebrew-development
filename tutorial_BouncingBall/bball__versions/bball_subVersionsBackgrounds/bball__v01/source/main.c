
#include <nds.h>
#include <stdio.h>

//Referenias Graficas que se generan en "build" por el Makefile "automágicamente"
#include "gfx_ball.h"
#include "gfx_brick.h"
#include "gfx_gradient.h"

//Entradas tile
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

void setupGraphics();
void update_logic();	
void update_graphics();

int main( void )
{
	//irqEnable( IRQ_VBLANK );	//Habilitar interrupcion vblank
	setupGraphics();
	while(1)
	{
		//Periodo de Renderizado
		//Actualizacion objetos del juego (Moverlos alrededor,                        
                // calcular velocidades, etc) 
		update_logic();
		
		//Espera para periodo vblank
		swiWaitForVBlank();
		
		//Periodo vblank: (seguro modificar graficos)
		//mover graficos alrededor
		update_graphics();
	}
}



void setupGraphics( void )
{
    // 1
    vramSetBankE( VRAM_E_MAIN_BG );
    vramSetBankF( VRAM_F_MAIN_SPRITE );
      
    
    // 2
    //generar el primer "tile" por rellenado a ceros
    int n;
    for( n = 0; n < 16; n++ )
    {
       BG_GFX[n] = 0;
    }
    
    //Copiando graficos
    dmaCopyHalfWords( 3, gfx_brickTiles, tile2bgram( tile_brick ), gfx_brickTilesLen );
    dmaCopyHalfWords( 3, gfx_gradientTiles, tile2bgram( tile_gradient ), gfx_gradientTilesLen );
    
    // 3
    //Paleta direccionada a la memoria de paleta
    dmaCopyHalfWords( 3, gfx_brickPal, pal2bgram( pal_bricks ), gfx_brickPalLen );
    dmaCopyHalfWords( 3, gfx_gradientPal, pal2bgram( pal_gradient ), gfx_gradientPalLen );
	
    //Asignar Color de fondo
    BG_PALETTE[0] = backdrop_colour;

 
    // 4
    videoSetMode( MODE_0_2D );


}

void update_logic()
{
}

void update_graphics()
{
}