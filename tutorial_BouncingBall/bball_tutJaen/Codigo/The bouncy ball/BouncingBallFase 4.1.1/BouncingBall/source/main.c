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
#define tile2bgram(t)	(BG_GFX + (t) *16)

//Paletas de entrada
#define pal_bricks		0 //paleta brick (entrada 0->15)
#define pal_gradient	1 //paleta gradient (entrada 16->31)

#define backdrop_colour	RGB8( 190, 255, 255 )
#define pal2bgram(p)	(BG_PALETTE + (p) * 16)

#define bg0map	((u16*)BG_MAP_RAM(1))
#define bg1map	((u16*)BG_MAP_RAM(2))

#define tiles_ball		0 //Tiles bola (16x16 tile 0->3)
#define tile2objram(t)	(SPRITE_GFX + (t) * 16)

#define pal_ball		0 //Paleta de la bola (entrada 0->15
#define pal2objram(p)	(SPRITE_PALETTE + (p) * 16)

typedef struct t_spriteEntry
{
	u16 attr0;
	u16 attr1;
	u16 attr2;
	u16 affine_data;
} spriteEntry;

int Nsprite;
touchPosition touch;

int x, y, px, py;

#define sprites ((spriteEntry*)OAM)

void update_logic()
{
	scanKeys();
	int keysh = keysHeld();
	
	touchRead(&touch);
	
	if( keysh & KEY_TOUCH )			//Chequea si UP esta presionado
	{
		x = touch.px;//valor x
		px = touch.rawx;
		y = touch.py; //valor y
		py = touch.rawy;
	}
	Nsprite += 1;
	
}
void update_graphics()
{
		sprites[Nsprite].attr0 = y ;//valor x
		sprites[Nsprite].attr1 = x + ATTR1_SIZE_16; //valor y
		sprites[Nsprite].attr2 = tiles_ball + (pal_ball << 12);
		iprintf("\x1b[10;5Hx: %d , %d\n", x, px);
		iprintf("\x1b[12;5Hy: %d , %d\n", y, py);
}

void setupGraphics( void )
{
	vramSetBankE( VRAM_E_MAIN_BG );
	vramSetBankF( VRAM_F_MAIN_SPRITE );
	
	consoleDemoInit();
	
	//leyenda
	iprintf("\n\n\n\n\n");
	iprintf("\tTouchPad: Crea una bola\n\n");
	
	Nsprite = 0;
	
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
	
	for(n = 0; n < 128; n++)
	{
		sprites[n].attr0 = ATTR0_DISABLED;
	}
	
	/*//Codigo de test para probar el motor de sprites
	for(n = 0; n <50; n++)
	{
		//atributo0: fijar la posicion vertical 0-> pantall_alto-sprite_alto
		//otras opciones por defecto que estarian bien (default == zeroed)
		sprites[n].attr0 = rand() % (192-16);
		
		//atributo1: fijar la posicion horizontal 0-> pantalla_ancho-sprite_ancho
		//establecer modode tamaño 16x16
		sprites[n].attr1 = (rand() % (256-16)) + ATTR1_SIZE_16;
		
		//atributo2: seleccionar numero de tile y numero paleta
		sprites[n].attr2 = tiles_ball + (pal_ball << 12);
	}
	*/
	
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
	
	videoSetMode( MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D_LAYOUT );
	
	//Copiando graficos
	dmaCopyHalfWords( 3, gfx_ballTiles, tile2objram( tiles_ball ), gfx_ballTilesLen );
	dmaCopyHalfWords( 3, gfx_brickTiles, tile2bgram( tile_brick ), gfx_brickTilesLen );
	dmaCopyHalfWords( 3, gfx_gradientTiles, tile2bgram( tile_gradient ), gfx_gradientTilesLen );
	
	//Paleta direccionada a la memoria de paleta
	dmaCopyHalfWords( 3, gfx_ballPal, pal2objram( pal_ball ), gfx_ballPalLen );
	dmaCopyHalfWords( 3, gfx_brickPal, pal2bgram( pal_bricks ), gfx_brickPalLen );
	dmaCopyHalfWords( 3, gfx_gradientPal, pal2bgram( pal_gradient ), gfx_gradientPalLen );
	
	//Asignar Color de fondo
	BG_PALETTE[0] = backdrop_colour;
	
	//libnds prefijos del registro de nombres con REG_
	REG_BG0CNT  = BG_MAP_BASE(1);
	REG_BG1CNT  = BG_MAP_BASE(2);
	
	REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG1 | BLEND_DST_BACKDROP;
	REG_BLDALPHA = (4) + (16<<8);
	
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
