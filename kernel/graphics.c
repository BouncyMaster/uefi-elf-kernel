#include <int.h>

void
draw_rect(uint32 * const framebufferPointer, const uint32 pixelsPerScanline,
	const uint16 x, const uint16 y, const uint16 width, const uint16 height,
	const uint32 color)
{
	// Pointer to the current pixel in the buffer
	uint32 *at;
	uint16 col = 0;

	for (uint16 row = 0; row < height; row++){
		for (col = 0; col < width; col++){
			at = framebufferPointer + x + col;
			at += ((y + row) * pixelsPerScanline);

			*at = color;
		}
	}
}
