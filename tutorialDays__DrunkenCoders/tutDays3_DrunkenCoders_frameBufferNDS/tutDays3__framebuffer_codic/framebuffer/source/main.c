/*---------------------------------------------------------------------------------
  Adaptació al còdic de "Frame buffer...finally" 

	$Id: main.cpp,v 1.13 2008-12-02 20:21:20 dovoto Exp $

	Simple console print demo
	-- dovoto


---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
 
int main(void)
{
	int i;
 
	//initialize the DS Dos-like functionality
	consoleDemoInit();
 
	//set frame buffer mode 0
	videoSetMode(MODE_FB0);
 
	//enable VRAM A for writing by the cpu and use 
	//as a framebuffer by video hardware
	vramSetBankA(VRAM_A_LCD);
 
	while(1)
	{
		u16 color = RGB15(31,0,0); //red
 
		scanKeys();
		int held = keysHeld();
 
		if(held & KEY_A)
			color = RGB15(0,31,0); //green
		
		if (held & KEY_X)
			color = RGB15(0,0,31); //blue
 
		swiWaitForVBlank();
 
		//fill video memory with the chosen color
		for(i = 0; i < 256*192; i++)
			VRAM_A[i] = color;
	}
 
	return 0;
}
 
