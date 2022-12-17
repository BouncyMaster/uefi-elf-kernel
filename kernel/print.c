#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include "ssfn.h"
#include "kernel.h"

/*
 * s - string
 * c - char
 */

extern ssfn_font_t _binary_font_sfn_start;

void
print_init(Video_Mode_Info *video)
{
	ssfn_src = &_binary_font_sfn_start;

	ssfn_dst.ptr = (u8 *)video->framebuffer;
	ssfn_dst.w = video->xRes;
	ssfn_dst.h = video->yRes;
	ssfn_dst.p = video->xRes * 4; // 4 bytes per pixel
	ssfn_dst.x = ssfn_dst.y = 0;
	ssfn_dst.fg = 0xFFFFFF;
}

// Prints a string to the screen
void
print_s(char *s)
{
	while (*s)
		ssfn_putc(*s++);
}
