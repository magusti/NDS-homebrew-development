#include <nds.h>

int main( void )
{
	irqEnable( IRQ_VBLANK );	//Habilitar interrupcion vblank
	while(1)
	{
		swiWaitForVBlank();
	}
	return 0;
}
