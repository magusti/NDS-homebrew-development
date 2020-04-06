#include <nds.h>
#include <stdio.h>

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

#define bg0map	((u16*)BG_MAP_RAM(1))
#define bg1map	((u16*)BG_MAP_RAM(2))

int punteroL = 112;
int punteroG = 0;
int fondo = 0;

void update_logic(){}
void update_graphics()
{
	/**Añadido al original**/
	scanKeys();
	int keysd = keysDown();
	int keysh = keysHeld();
	/**Añadido al original**/
	
	/**Añadido al original**/
	if(keysd & KEY_L)
	{
		if (fondo == 0 ) fondo = 1;
		else fondo = 0;
	}
	
	if(keysd & KEY_R)
	{
		if (fondo == 0 )
		{
			bgSetPriority(0,0);
			bgSetPriority(1,1);
		}
		else
		{ 
			bgSetPriority(0,1);
			bgSetPriority(1,0);
		}
	}
	
	if(keysd & KEY_A)
	{
		if(fondo == 0) bgHide(0);
		else bgHide(1);
		
	}
	if(keysd & KEY_B)
	{
		if(fondo == 0) bgShow(0);
		else bgShow(1);

	}
	if(keysh & KEY_UP)
	{
		if(fondo == 0)
		{
			REG_BG0VOFS = punteroL;
			punteroL++;
		}
		else
		{
			REG_BG1VOFS = punteroG;
			punteroG++;
		}
	}
	if(keysh & KEY_DOWN)
	{
		if(fondo == 0)
		{
			REG_BG0VOFS = punteroL;
			punteroL--;
		}
		else
		{
			REG_BG1VOFS = punteroG;
			punteroG--;
		}
	}
	
	
	
		if(fondo == 0) iprintf("\x1b[20;11HLadrillos");
		else iprintf("\x1b[20;11HGradiente");
		
	/**Añadido al original**/
}

void setupGraphics( void )
{
	/**Añadido al original**/
	consoleDemoInit();
	
	//leyenda
	iprintf("\x1b[4;8HPrueba Sistema 2");
	iprintf("\x1b[8;1HBoton L: Seleccionar Fondo");
	iprintf("\x1b[10;1HBoton R: Asignar Max. Prioridad");
	iprintf("\x1b[12;1HBoton A: Quita Fondo Elegido");
	iprintf("\x1b[14;1HBoton B: Pone Fondo Elegido");
	iprintf("\x1b[16;1HUP: Sube Fondo Elegido");
	iprintf("\x1b[18;1HDOWN: Baja Fondo Elegido");
	/**Añadido al original**/
	
	vramSetBankE( VRAM_E_MAIN_BG );
	vramSetBankF( VRAM_F_MAIN_SPRITE );
	
	//generar el primer banco de tile por borrado a cero
	int n;
	for( n = 0; n < 16; n++ )
	{
		BG_GFX[n] = 0;
	}
	
	for(n = 0; n < 1024; n++)
	{
		bg0map[n] = 0;
		bg1map[n] = 0;
	}
	
	int x, y;
	for( x = 0; x < 32; x++ )
	{
		for( y = 0; y < 6; y++ )
		{
			//formula magica para saber si el tile tiene que voltearse
			int hflip = (x & 1) ^ (y & 1);
			
			//establecer la entrada tilemap
			bg0map[x + y * 32] = tile_brick | (hflip << 10) | (pal_bricks << 12);
			
		}
		
		REG_BG0VOFS = 112;
		
		for( y = 0; y < 8; y++ )
		{
			int tile = tile_gradient +y;
			bg1map[ x + y * 32 ] = tile | (pal_gradient << 12);
		}
	}
	
	videoSetMode( MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE);
	
	
	//Copiando graficos
	dmaCopyHalfWords( 3, gfx_brickTiles, tile2bgram( tile_brick ), gfx_brickTilesLen );
	dmaCopyHalfWords( 3, gfx_gradientTiles, tile2bgram( tile_gradient ), gfx_gradientTilesLen );
	
	//Paleta direccionada a la memoria de paleta
	dmaCopyHalfWords( 3, gfx_brickPal, pal2bgram( pal_bricks ), gfx_brickPalLen );
	dmaCopyHalfWords( 3, gfx_gradientPal, pal2bgram( pal_gradient ), gfx_gradientPalLen );
	
	//Asignar Color de fondo
	BG_PALETTE[0] = backdrop_colour;
	
	//libnds prefijos del registro de nombres con REG_
	REG_BG0CNT  = BG_MAP_BASE(1);
	REG_BG1CNT  = BG_MAP_BASE(2);
	
}

int main( void )
{
	//irqInit();					//inicializar interrupciones
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
