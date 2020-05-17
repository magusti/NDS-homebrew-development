
@{{BLOCK(gfx_brick)

@=======================================================================
@
@	gfx_brick, 8x8@4, 
@	+ palette 3 entries, not compressed
@	+ 1 tiles not compressed
@	Total size: 6 + 32 = 38
@
@	Time-stamp: 2015-05-14, 13:12:01
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.12
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global gfx_brickTiles		@ 32 unsigned chars
	.hidden gfx_brickTiles
gfx_brickTiles:
	.word 0x22222222,0x22222222,0x21111111,0x21111111,0x21111111,0x21111111,0x21111111,0x21111111

	.section .rodata
	.align	2
	.global gfx_brickPal		@ 8 unsigned chars
	.hidden gfx_brickPal
gfx_brickPal:
	.hword 0x001F,0x1D74,0x77BD

@}}BLOCK(gfx_brick)
