#include <nds.h>

/**Añadido al original**/
#include <stdio.h>
/**Añadido al original**/

#include "ball.h"

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

ball g_ball;

typedef struct t_spriteEntry
{
	u16 attr0;
	u16 attr1;
	u16 attr2;
	u16 affine_data;
} spriteEntry;

#define sprites ((spriteEntry*)OAM)

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
	
	for(n = 0; n < 1024; n++)
	{
		bg0map[n] = 0;
		bg1map[n] = 0;
	}
	
	for(n = 0; n < 128; n++)
	{
		sprites[n].attr0 = ATTR0_DISABLED;
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
	
	/**Añadido al original**/
	//Leyenda
	consoleDemoInit();
	
	//Texto a representar por defecto se procesan en la pantalla inferior
	iprintf("\x1b[8;8HPrueba sistema 4");
	iprintf("\x1b[10;3HBoton A: Aumentar la Bola");
	iprintf("\x1b[12;3HBoton B: Disminuir la Bola");
	iprintf("\x1b[14;3HBoton X: Rotar a derecha");
	iprintf("\x1b[16;3HBoton Y: Rotar a izquierda");
	iprintf("\x1b[18;3HBoton START: Reiniciar Bola");
	iprintf("\x1b[20;5HPad: Control de la Bola");
	/**Añadido al original**/
}

//Configuracion del manejador de interrupciones con vblank irq habilitado
void setupInterrupts( void )
{
	
	//Habilitar interrupcion vblank (necesario para swiWAitForVBlank)
	irqEnable( IRQ_VBLANK );
}

//Resetear atributos del balon
void resetBall( void )
{
	//usar indice sprite 0 (0->127)
	g_ball.sprite_index = 0;
	
	//usar matriz de afinamiento 0 (0->31)
	g_ball.sprite_affine_index = 0;
	//g_ball.sprite_affine_indexS = 0;
	//g_ball.sprite_affine_indexR = 1;
	
	//X = 128.0
	g_ball.x = 128 << 8;
	
	//Y = 64.0
	g_ball.y = 64 << 8;
	
	//iniciar velocidad X un bit a la derecha
	g_ball.xvel = 100 << 4;
	
	//resetear velocidad Y
	g_ball.yvel = 0;
	
	/**Añadido al original**/
	//resetear la altura de la bola a c_diam 32
	g_ball.height = 32 << 8;
	
	//resetear angulo de la bola
	g_ball.alpha = 0 << 8;
	
	//resetear rotacion o escalado: por defecto escalado
	g_ball.scale = true;
	/**Añadido al original**/
}

//Informacion Actualizacion grafica (llamar durante vblank)
void updateGraphics( void )
{
	/**Añadido al original**/
	if(g_ball.scale)
	{
		g_ball.sprite_affine_index = 0;
	}
	else
	{
		g_ball.sprite_affine_index = 1;
	}
	/**Añadido al original**/
	
	//Actualizar sprite del balon, camara = 0, 0
	ballRender( &g_ball, 0, 0);
}
void processInput( void )
{
	/**Añadido al original**/
	scanKeys();
	
	int keysh = keysHeld();
	int keysp = keysDown();
	//proceso uso de entradas
	if( keysh & KEY_UP)			//Chequea si UP esta presionado
	{
		g_ball.y -= 1 << 8;
	}
	if( keysh & KEY_DOWN )		//chequea si DOWN esta presionado
	{
		g_ball.y += 1 << 8;
	}
	if( keysh & KEY_LEFT )		//chequea si LEFT esta presionado
	{
		g_ball.x -= 1 << 8;
	}
	if( keysh & KEY_RIGHT )		//chequea si RIGHT esta presionado
	{
		g_ball.x += 1 << 8;
	}
	if( keysh & KEY_A )		//chequea si A esta presionado
	{
		g_ball.height -= 1 << 8;
		if(!g_ball.scale)g_ball.scale = true;
	}
	if( keysh & KEY_B )		//chequea si B esta presionado
	{
		g_ball.height += 1 << 8;
		if(!g_ball.scale)g_ball.scale = true;
	}
	if(keysp & KEY_START)
	{
		resetBall();
	}
	if(keysp & KEY_X)
	{
		g_ball.alpha += 9 << 8;
		if(g_ball.scale)g_ball.scale = false;
	}
	if(keysp & KEY_Y)
	{
		g_ball.alpha -= 9 << 8;
		if(g_ball.scale)g_ball.scale = false;
	}
	/**Añadido al original**/
}

void updateLogic( void )
{
	/**Añadido al original**/
	processInput();
	ballUpdate( &g_ball );
	/**Añadido al original**/
}

int main( void )
{
	//Cosas de configuracion
	setupInterrupts();
	setupGraphics();
	resetBall();
	
	//Bucle Principal
	while(1)
	{
		//Periodo de Renderizado
		//Actualizacion objetos del juego (Moverlos alrededor, calcular velocidades, etc)
		updateLogic();
		
		//Espera para periodo vblank
		swiWaitForVBlank();
		
		//Periodo vblank: (seguro modificar graficos)
		//mover graficos alrededor
		updateGraphics();
	}
	return 0;
}
