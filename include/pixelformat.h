#pragma once

/// all supported formats (including shifts)
/**

There's something wacked out about these format names :

	for 16 bit & 32 bit , the _RGB or _BGR refers to their order
		*in the word or dword* ; since we're on intel, this means
		the bytes in the data file have the *opposite* order !!
		(for example the 32 bit _ARGB is actually B,G,R,A in raw bytes)
	for 24 bit , the _RGB or _BGR refers to their order in the
		actual bytes, so that windows bitmaps actually have
		_RGB order in a dword !!

* YUV : the pixelformat ops here are identical to those of 24bit_RGB ;
		this is just a place-keeper to notify you that you should to a YUV_to_RGB conversion

*/
enum class PixelFormat
{
	GE_PIXELFORMAT_NO_DATA = 0,
	GE_PIXELFORMAT_8BIT,	  // PAL
	GE_PIXELFORMAT_8BIT_GRAY, // no palette (intensity from bit value)
	GE_PIXELFORMAT_16BIT_555_RGB,
	GE_PIXELFORMAT_16BIT_555_BGR,
	GE_PIXELFORMAT_16BIT_565_RGB, // #5
	GE_PIXELFORMAT_16BIT_565_BGR,
	GE_PIXELFORMAT_16BIT_4444_ARGB, // #7
	GE_PIXELFORMAT_16BIT_1555_ARGB,
	GE_PIXELFORMAT_24BIT_RGB, // #9
	GE_PIXELFORMAT_24BIT_BGR,
	GE_PIXELFORMAT_24BIT_YUV, // * see note below
	GE_PIXELFORMAT_32BIT_RGBX,
	GE_PIXELFORMAT_32BIT_XRGB,
	GE_PIXELFORMAT_32BIT_BGRX,
	GE_PIXELFORMAT_32BIT_XBGR,
	GE_PIXELFORMAT_32BIT_RGBA,
	GE_PIXELFORMAT_32BIT_ARGB, // #17
	GE_PIXELFORMAT_32BIT_BGRA,
	GE_PIXELFORMAT_32BIT_ABGR,

	GE_PIXELFORMAT_WAVELET, // #20 , Wavelet Compression

	GE_PIXELFORMAT_COUNT,
	GE_PIXELFORMAT_8BIT_PAL = GE_PIXELFORMAT_8BIT
};
