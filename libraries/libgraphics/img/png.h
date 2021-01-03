#ifndef __GRAPHICS_PNG_H
#define __GRAPHICS_PNG_H 1

#include <stdint.h>
#include <stddef.h>

int png_check_signature(uint8_t *sig, size_t length);

#endif

/**
 * Sources:
 *  http://www.libpng.org/pub/png/book/chapter08.html
 * 
 * PNG Signature(8 Bytes)
 * ----------------------
 *  0x89
 *  0x50(P)
 *  0x4e(N)
 *  0x47(G)
 *  0x0d(Carriage feed)
 *  0x0a(Line Feed)
 *  0x1a(Substitute/CTRL-Z)
 *  0x0a(Line Feed)
 * 
 * IHDR (Image Header)
 * ------------------
 * 
 * PLTE (Palette)
 * --------------
 * 
 * tRNS (Transparency)
 * -------------------
 * 
 * IDAT (Image Data)
 * -----------------
 * 
 * IEND (Image End)
 * ----------------
 * 0x00000000(Length)
 * 0x444e4549(IEND)
 * Na (Data)
 * 0x826042ae(CRC??)
 * 
 * */
