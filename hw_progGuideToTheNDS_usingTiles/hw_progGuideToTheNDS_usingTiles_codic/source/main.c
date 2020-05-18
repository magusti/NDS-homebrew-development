/*
 * magusti@ultravioleta:~/docencia/2k18-2k19/AEV/traspes/tema3_imatges/mapes/tutorial_mapes/3erPas_carregarTeseles/homebrewProgrammersGuideToTheNDS_UsingTiles$ 

 Recreació de "Homebrew Programmers Guide to the Nintendo DS
0.1 . Part II: Graphics Basisc. Using Tiles"
 <http://dspassme.jzdocs.com/programmers_guide/tutorial/using_tiles.html>

 */

#include <nds.h>
#include <stdio.h>

// Gràfics
#include "alpha.h"
/*
#define alphaTilesLen 1664
extern const unsigned int alphaTiles[416];

#define alphaPalLen 32
extern const unsigned short alphaPal[16];
 */
#include "tiles.h"
/*
#define tilesTilesLen 2048
extern const unsigned int tilesTiles[512];

#define tilesPalLen 32
extern const unsigned short tilesPal[16];
 */


/*
 *
 *
#define C11 0   
#define C12 1   
#define C13 2   
#define C14 3 
#define C21 4  

#define C22 5   
#define C23 6   
#define C24 7   
#define P11 8  

#define P12 9  
#define P21 12  
#define P22 13  
#define T11 10 

#define T12 11  
#define T21 14  
#define T22 15  
*/
#define C11 0   
#define C12 1   
#define C13 2   
#define C14 3

#define C21 4  
#define C22 5   
#define C23 6   
#define C24 7

#define P11 8  
#define P12 9  
#define T11 10 
#define T12 11  

#define P21 12  
#define P22 13  
#define T21 14  
#define T22 15 

//void Tutorial01Tile::drawMap(u16* map)
void drawMap( u16* map )
{
  u16 toDraw[] = {
    C11,C12,C13,C14,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    P11,P12,T11,T12,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    C21,C22,C23,C24,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,
    P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,C11,C12,C13,C14,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,

    P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,C21,C22,C23,C24,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,
    P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

  };

  u16 original__toDraw[] = {
    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,
    P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,C11,C12,C13,C14,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,

    P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,C21,C22,C23,C24,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,P11,P12,
    P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,P21,P22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

    T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,P11,P12,P11,P12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,T11,T12,
    T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,P21,P22,P21,P22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,T21,T22,

  };

 
  for(u16 i = 0; i < sizeof(toDraw) / sizeof(u16); ++i) {
    map[i] = original__toDraw[i];
    //  map[i] = toDraw[i];
  }  
 
 //Si me pases el número ... dmaCopy(toDraw,  bgGetMapPtr( bg ), sizeof(toDraw) / sizeof(u16));
 //dmaCopy(toDraw,  map, sizeof(toDraw) / sizeof(u16));  
 
} // end Tutorial01Tile::drawMap


/*
 * //void Tutorial01Tile::updateDisplay(void) {
void updateDisplay(void) {
}
*/
/*
static void InterruptHandler(void) {
  if (IF & IRQ_VBLANK) {
    //Tutorial01Tile& app(Tutorial01Tile::theApp());

    updateDisplay();

    IF = IRQ_VBLANK;
  }
}
// end InterruptHandler
*/

//int main(int argc, char** argv) {
int main( void )
{
  unsigned int i, j;
  const int char_base = 0;
  const int screen_base = 20;  //10 // map_base
  int elFondo;
  
  //lcdMainOnBottom(); // ó 
  lcdMainOnTop();

  
  
  vramSetBankA(VRAM_A_MAIN_BG);
  videoSetMode( MODE_0_2D | DISPLAY_BG0_ACTIVE );
  // enable background 0 in 256 color mode with a 256x256 map
  // BG_TILE_BASE changes the offset where tile data is stored
  // BG_MAP_BASE gives the offset to the map data
  //devkitpro/examples/nds/Graphics/grit/256colorTilemap/source/main.c
  //	  BGCTRL[0] = BG_TILE_BASE(1) | BG_MAP_BASE(0) | BG_COLOR_256 | BG_32x32;
  /* Han hagut canvits a libnds
   BG0_CR = BG_256_COLOR | BG_TILE_BASE(char_base) | BG_MAP_BASE(screen_base);
   * 
   error: 'BG0_CR' undeclared (first use in this function); did you mean 'DMA_CR'?
   */
  
  /*
   Initializes a background on the sub display Sets up background control register with specified settings and defaults to 256 color mode for tiled backgrounds. Sets the rotation/scale attributes for rot/ex rot backgrounds to 1:1 scale and 0 angle of rotation.

Parameters
    layer	background hardware layer to init. Must be 0 - 3
    type	the type of background to init
    size	the size of the background
    mapBase	the 2k offset into vram the tile map will be placed
    –OR–
    the 16k offset into vram the bitmap data will be placed for bitmap backgrounds
    tileBase	the 16k offset into vram the tile graphics data will be placed

Returns
    the background id to be used in the supporting functions  
   */
  elFondo = bgInit(0,       // index del BG en Main --> bgInitSub per a l'atre
                   BgType_Text8bpp, BgSize_T_256x256,   // tipo i tamany
                   screen_base,       // map 
                   char_base);      // tile
  u16* tile = (u16*)CHAR_BASE_BLOCK(char_base);
  u16* map = (u16*)SCREEN_BASE_BLOCK(screen_base);
  /*
  PrintConsole *pantallaEscena = consoleInit(0,0, //ptrAConsola o NULL, #BG
                                             BgType_Text8bpp, BgSize_T_256x256, // Tammé BgSize_T_512x512
                                        screen_base, //map, //BG_MAP_BASE(screen_base),
                                        char_base, //tile, //BG_TILE_BASE(char_base), 
                                        true, false);  // CORE_MAIN, NO_FONT_PER_DEFECTE
//                                        screen_base, char_base, true, false);
  */
  
  //Carregar les teseles
  /*
   * 
   * for(i = 0; i < tilesTilesLen; ++i) {    
    tile[i] = tilesTiles[i];
  }
  dmaCopy(tilesTiles,  tile, tilesTilesLen);
  */
  dmaCopy(tilesTiles,  bgGetGfxPtr(elFondo), tilesTilesLen);
  
  /*
  for(u16 i = 0; i < tilesPalLen; ++i) {    
    BG_PALETTE[i] = tilesPal[i];    
  }
  */
  dmaCopy(tilesPal,  BG_PALETTE, tilesPalLen);
  
  // Draw the forest scene: copiar els gràfics a la VRAM
  //drawMap( map );
  drawMap( bgGetMapPtr(elFondo) );
  
  /*
  dmaCopy(tilesTiles, bgGetGfxPtr(pantallaEscena), tilesTilesLen);
  dmaCopy(tilesPal, BG_PALETTE, tilesPalLen);
  //
  dmaCopy(toDraw, bgGetMapPtr(pantallaEscena), sizeof(toDraw) ); // Si fas "toDraw" una variable accesible a esta cridà
  */
  
  vramSetBankC(VRAM_C_SUB_BG);
  videoSetModeSub( MODE_0_2D | DISPLAY_BG0_ACTIVE );

 // BGCTRL_SUB[0] = GFX_COLOR | BG_TILE_BASE(char_base) | BG_MAP_BASE(screen_base);
 // Ara deuria ser algo com: PrintConsole *console = consoleInit(0,0, BgType_Text4bpp, BgSize_T_256x256, map_base, tile_base, false, false); ???
  u16* sub_tile = (u16*)CHAR_BASE_BLOCK_SUB(char_base);
  u16* sub_map = (u16*)SCREEN_BASE_BLOCK_SUB(screen_base);
  PrintConsole *pantallaMsg = consoleInit(0,0, BgType_Text4bpp, BgSize_T_256x256, 
                                      screen_base, char_base, 
                                      false, false); // CORE_SUB, NO_FONT_PER_DEFECTE
  /*
   Parameters
    console	A pointer to the console data to initialze (if it's NULL, the default console will be used)
    layer	background layer to use
    type	the type of the background
    size	the size of the background
    mapBase	the map base
    tileBase	the tile graphics base
    mainDisplay	if true main engine is used, otherwise false
    loadGraphics	if true the default font graphics will be loaded into the layer
   */

	ConsoleFont font;

	font.gfx = (u16*)alphaTiles;
	font.pal = (u16*)alphaPal;
	font.numChars = 95;
	font.numColors =  alphaPalLen / 2;
	font.bpp = 4;
	font.asciiOffset = 32;
	font.convertSingleColor = false;
	
	consoleSetFont(pantallaMsg, &font);
  
  BG_PALETTE_SUB[0] = RGB15(0, 0, 0);
  BG_PALETTE_SUB[1] = RGB15(0, 31, 0);


// Per cert: "page not found" per a <http://dspassme.com/programmers_guide/>
  // Tile information loaded
  char* msg[] = {
    "",
    "Tile Demo.",
    "tile01 project.",
    "",
    "http://www.dspassme.com/programmers_guide/",
    "http://dspassme.jzdocs.com/programmers_guide/tutorial/", // Este sí
    "",
    "PhoenixRising",
  //01234567890123456789012345678901
  };
    for(i = 0; i < 8; ++i) {
	  iprintf("%s\n", msg[i]);
    }
  
  /*
    // Load our tile information
  //  for(i = 0; i < sizeof(alpha8Data) / sizeof(u16); ++i) {
  for(i = 0; i < alphaTilesLen; ++i) {    
    //    sub_tile[i] = alpha8Data[i];
    sub_tile[i] = alphaTiles[i];
  }
  //for(i = 0; i < sizeof(alpha8Palette) / sizeof(u16); ++i) {
 for(i = 0; i < alphaTilesLen; ++i) {
   //    BG_PALETTE_SUB[i] = alpha8Palette[i];
    BG_PALETTE_SUB[i] = alphaPal[i];
  }
    
  BG_PALETTE_SUB[0] = RGB15(0, 0, 0);
  BG_PALETTE_SUB[1] = RGB15(0, 31, 0);
  
  // Draw the message on the screen.
  for(i = 0; i < sizeof(msg) / sizeof(char*); ++i) {
    for(j = 0; msg[i][j] != '\0'; ++j) {
      sub_map[j + (i * 32)] = msg[i][j] - ' ';
    }
  }
*/
   // Per alternar la activa, per a fer printfs en ella:
   // consoleSelect( pantallaMsg );  o  consoleSelect( pantallaEscena ); 
   
	// Pots no fer esta espera, que no estava en l'original.
    while(1) {
		swiWaitForVBlank();
		scanKeys();
		int keys = keysDown();
		if(keys & KEY_START) break;
	}

  return 0;
}

