/*---------------------------------------------------------------------
  Adaptació del còdic de Tutorial Days 3: "Circles" 

  ---------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>


void  DrawLine(int x1, int y1, int x2, int y2, unsigned short color) {
  int yStep = SCREEN_WIDTH;
  int xStep = 1;
  int xDiff = x2 - x1;
  int yDiff = y2 - y1;

  int errorTerm = 0;
  int offset = y1 * SCREEN_WIDTH + x1;
  int i;

  //need to adjust if y1 > y2
  if (yDiff < 0)    {
      yDiff = -yDiff;       //absolute value
      yStep = -yStep;       //step up instead of down
    }

  //same for x
  if (xDiff < 0)    {
      xDiff = -xDiff;
      xStep = -xStep;
    }
  
  //case for changes more in X than in Y
  if (xDiff > yDiff)   {
      for (i = 0; i < xDiff + 1; i++)	{
	  VRAM_A[offset] = color;
	  offset += xStep;
	  errorTerm += yDiff;
	  if (errorTerm > xDiff)	    {
	      errorTerm -= xDiff;
	      offset += yStep;
	    }
	}
    }//end if xdiff > ydiff
  //case for changes more in Y than in X
  else    {
      for (i = 0; i < yDiff + 1; i++)	{
	  VRAM_A[offset] = color;
	  offset += yStep;
	  errorTerm += xDiff;
	  if (errorTerm > yDiff) 	    {
	      errorTerm -= yDiff;
	      offset += xStep;
	    }
	}
    }

} // DrawLine

int esferic = 8;

void DrawPixel(int X, int Y, unsigned short Color) {
  int i;

  i= X + Y * SCREEN_WIDTH;
  if (Color == 0) VRAM_A[i] = RGB15(0,0,0);
  else
    if (Color == 1) VRAM_A[i] = RGB15(31,31,31);
} // DrawPixel


void DrawCircle (int rayon, int x_centre, int y_centre, unsigned short Color) {
 int x, y, m;

 x = 0;
 y = rayon;

 // Place on the top of the circle
 m = 10 - 1*rayon; //5 - 4*rayon ;
 // initialisation
 while (x <= y) {
  // while we are in the second half
  DrawPixel(  x+x_centre,  y+y_centre, Color );
  DrawPixel(  y+x_centre,  x+y_centre, Color );
  DrawPixel( -x+x_centre,  y+y_centre, Color );
  DrawPixel( -y+x_centre,  x+y_centre, Color );
  DrawPixel(  x+x_centre, -y+y_centre, Color );
  DrawPixel(  y+x_centre, -x+y_centre, Color );
  DrawPixel( -x+x_centre, -y+y_centre, Color );
  DrawPixel( -y+x_centre, -x+y_centre, Color );

  
  DrawLine( x+x_centre, y+y_centre, -x+x_centre, -y+y_centre, Color );
  DrawLine( y+x_centre, x+y_centre, -y+x_centre, -x+y_centre, Color );
  DrawLine(-x+x_centre, y+y_centre,  x+x_centre, -y+y_centre, Color );
  DrawLine(-y+x_centre, x+y_centre,  y+x_centre, -x+y_centre, Color );
 
  
  if (m > 0) { //choix du point F
    y = y - 1;
    m = m - esferic*y; // m - 8*y;
  }
  x = x + 1;
  m = m + 8*x + 4;
 } // while
} // DrawCircle

#define MIN_RADIO 5
#define MAX_RADIO 100


void borrarPantalla() {
  int i;
  for (i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) VRAM_A[i] = 0;
} // borrarPantalla

int main( void ) {

  touchPosition touch;
  int radio = 5;
  
  //  int oldX = 0;
  //  int oldY = 0;

  //inicializa una pantalla en modo texto
  consoleDemoInit();
 
  //y la otra en modo raster ( framebuffer )
  videoSetMode(MODE_FB0);
  vramSetBankA(VRAM_A_LCD);

  lcdMainOnBottom();

  while(1)     {
    scanKeys();
    touchRead(&touch);
    if(keysDown() & KEY_UP) {
      radio = (radio+10) % MAX_RADIO;
      printf("Radio = %d\n", radio);
    }
    
    if(keysDown() & KEY_DOWN) {
      radio = (radio <= MIN_RADIO? 0 : (radio-10));    
      printf("Radio = %d\n", radio);
    }

    if(keysDown() & KEY_A) {
      borrarPantalla();
      printf("\n");
    }

    if(keysDown() & KEY_X) {
      esferic = (esferic <= 0? 0 : (esferic-10));    
      printf("Esferic = %d\n", esferic);
    }

    if(keysDown() & KEY_Y) {
      esferic = (esferic >= 100? 100 : (esferic+10));    
      printf("Esferic = %d\n", esferic);
    }

    if(!(keysDown() & KEY_TOUCH) && (keysHeld() & KEY_TOUCH)) {
      DrawCircle( radio, touch.px, touch.py, rand() );
     
    }
	//    oldX = touch.px;
	//    oldY = touch.py;
    swiWaitForVBlank();
  }
  return 0;
} // Main

