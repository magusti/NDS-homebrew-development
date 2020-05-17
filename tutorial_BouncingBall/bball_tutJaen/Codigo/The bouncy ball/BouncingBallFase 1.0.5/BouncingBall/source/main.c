#include <nds.h>
#include <stdio.h>
//#include <string.h>

//Referenias Graficas
#include "gfx_ball.h"
#include "gfx_brick.h"
#include "gfx_gradient.h"

//Entradas tile
#define tile_empty		0 //tile 0 = empty
#define tile_brick		1 //tile 1 = brick
#define tile_gradient	2 //tile 2 = gradient

//macro para calcular memoria BG VRAM
//direccion con el indice de tile
#define tile2bgram(t) (BG_GFX + (t) *16)

//Paletas de entrada
#define pal_bricks		0 //paleta brick (entrada 0->15)
#define pal_gradient	1 //paleta gradient (entrada 16->31)

#define backdrop_colour	RGB8( 190, 255, 255 )
#define pal2bgram(p)	(BG_PALETTE + (p) * 16)

/**Añadido al original**/
u32 keys;
int red = 0;
int green = 0;
int blue = 0;
/**Añadido al original**/

void update_logic(){}
void update_graphics()
{
	/**Añadido al original**/
	BG_PALETTE[0] = RGB8( red, green, blue );//Añadido Para cambiar el color de fondo
	
	scanKeys();
	
	int keys = keysDownRepeat(); // Se lee el estado actual de todos los botones.
	
	if(KEY_A & keys) //Solo si se pulsa el boton A
	{
		red = red + 5;
		if(red > 255)
		{
				red = 0;
		}
	}
	if(KEY_B & keys) //Solo si se pulsa el boton B
	{
		green = green + 5;
		if(green > 255)
		{
				green= 0;
		}
	}
	if(KEY_X & keys) //Solo si se pulsa el boton X
	{
		
		blue = blue + 5;
		
		if(blue > 255)
		{
				blue = 0;
		}

	}
	
	consoleDemoInit();
	
	//Texto a representar por defecto se procesan en la pantalla inferior
	iprintf("\x1b[8;8HPrueba sistema 1");//x1b[<linea>;<columna>H	linea entre 0-23 y columna entre 0-31
	iprintf("\x1b[10;1HBoton A: Incrementar 5 a Rojo");
	iprintf("\x1b[12;1HBoton B: Incrementar 5 a Verde");
	iprintf("\x1b[14;1HBoton X: Incrementar 5 a Azul");
	iprintf("\x1b[16;6HR: %d", red);
	iprintf("\x1b[16;13HG: %d", green);
	iprintf("\x1b[16;20HB: %d", blue);
	/**Añadido al original**/
}

void setupGraphics( void )
{
	vramSetBankE( VRAM_E_MAIN_BG );
	vramSetBankF( VRAM_F_MAIN_SPRITE );
	
	//generar el primer banco de tile por borrado a cero
	int n;
	for( n = 0; n < 16; n++ )
	{
		BG_GFX[n] = 0;
	}
	
	videoSetMode( MODE_0_2D );
	
	//Copiando graficos
	dmaCopyHalfWords( 3, gfx_brickTiles, tile2bgram( tile_brick ), gfx_brickTilesLen );
	dmaCopyHalfWords( 3, gfx_gradientTiles, tile2bgram( tile_gradient ), gfx_gradientTilesLen );
	
	//Paleta direccionada a la memoria de paleta
	dmaCopyHalfWords( 3, gfx_brickPal, pal2bgram( pal_bricks ), gfx_brickPalLen );
	dmaCopyHalfWords( 3, gfx_gradientPal, pal2bgram( pal_gradient ), gfx_gradientPalLen );
	
	//Asignar Color de fondo
	BG_PALETTE[0] = backdrop_colour;
	
}

int main( void )
{
	irqEnable( IRQ_VBLANK );	//HAbilitar interrupcion vblank
	setupGraphics();
	
	while(1)
	{
		//Periodo de Renderizado
		//Actualizacion objetos del juego (Moverlos alrededor, calcular velocidades, etc)
		update_logic();
		
		//Espera para periodo vblank
		swiWaitForVBlank();
		
		//Periodo vblank: (seguro modificar graficos)
		//mover graficos alrededor
		update_graphics();
	}
	//return 0;
}
