
@{{BLOCK(gfx_gradient)

@=======================================================================
@
@	gfx_gradient, 8x64@4, 
@	+ palette 16 entries, not compressed
@	+ 8 tiles not compressed
@	Total size: 32 + 256 = 288
@
@	Time-stamp: 2015-05-14, 13:12:02
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.12
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global gfx_gradientTiles		@ 256 unsigned chars
	.hidden gfx_gradientTiles
gfx_gradientTiles:
	.word 0x11111111,0x11111111,0x11111111,0x11111111,0x12121212,0x21212121,0x22222222,0x22222222
	.word 0x23332333,0x33333333,0x34343434,0x43434343,0x44444444,0x44444444,0x45454545,0x55545554
	.word 0x55565556,0x55655565,0x66566656,0x66656665,0x66676667,0x66766676,0x77677767,0x76767676
	.word 0x77777777,0x77777777,0x78787878,0x87878787,0x88888888,0x88888888,0x89898989,0x98989898
	.word 0x99999999,0x99999999,0x9A9A9A9A,0xA999A999,0x9AAA9AAA,0xA9AAA9AA,0xAAAAAAAA,0xAAAAAAAA
	.word 0xABABABAB,0xBABABABA,0xBBBBBBBB,0xBBBBBBBB,0xBCBCBCBC,0xCBCCCBCC,0xCCCCCCCC,0xDCCCDCCC
	.word 0xCDDDCDDD,0xDCDDDCDD,0xDEDDDEDD,0xEDEDEDED,0xEEEEEEEE,0xEEFEEEFE,0xFFFFFFFF,0xFFFFFFFF
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global gfx_gradientPal		@ 32 unsigned chars
	.hidden gfx_gradientPal
gfx_gradientPal:
	.hword 0x0000,0x56B5,0x4E73,0x4631,0x4210,0x39CE,0x35AD,0x294A
	.hword 0x2529,0x1CE7,0x14A5,0x0C63,0x0842,0x0421,0x0000,0x0000

@}}BLOCK(gfx_gradient)
