#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <int.h>

void draw_rect(uint32 * const framebufferPointer, const uint32 pixelsPerScanline,
	const uint16 x, const uint16 y, const uint16 width, const uint16 height,
	const uint32 color);

#endif // GRAPHICS_H
