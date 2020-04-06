
@{{BLOCK(gfx_ball)

@=======================================================================
@
@	gfx_ball, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2015-04-06, 16:10:44
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.12
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global gfx_ballTiles		@ 128 unsigned chars
	.hidden gfx_ballTiles
gfx_ballTiles:
	.word 0xCC200000,0x999E2000,0xBBBA9C00,0xB333B920,0x33333AE0,0x33333B92,0x33333B9C,0xB333BB9C
	.word 0x00000D2C,0x000D2759,0x00D2648A,0x012648AB,0x0DC548AB,0x1D7548AB,0x1D7548AB,0x1D7548AB
	.word 0xBBBBBA9C,0xAAAAA852,0x8888847D,0x44444620,0x555562D0,0x777C2D00,0xDDDD1000,0x11100000
	.word 0x1DC6548A,0x1D275448,0x112C6544,0x01D2C655,0x011D2C76,0x0011D22C,0x000111DD,0x00000111

	.section .rodata
	.align	2
	.global gfx_ballPal		@ 32 unsigned chars
	.hidden gfx_ballPal
gfx_ballPal:
	.hword 0x001F,0x0C83,0x1505,0x77FD,0x26AA,0x2229,0x1DE8,0x19A7
	.hword 0x372E,0x326D,0x4B73,0x5FB7,0x1946,0x10C4,0x25AA,0x0000

@}}BLOCK(gfx_ball)
