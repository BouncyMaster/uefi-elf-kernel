#include <int.h>

#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include "ssfn.h"
#include "boot.h"

extern ssfn_font_t _binary_font_sfn_start;
static char STR_OUT[20];

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

// Convert uint to string
const char *
u_to_str(u64 val)
{
	u8 size = 0;
	u64 sizeTest = val;
	while (sizeTest /= 10)
		size++;

	u8 index = 0;
	do {
		STR_OUT[size - index++] = val % 10 + '0';
	} while (val /= 10);

	STR_OUT[size + 1] = 0; 

	return STR_OUT;
}

// Prints a string to the screen
void
print(const char *s)
{
	while (*s)
		ssfn_putc(*s++);
}

void
print_lf(void)
{
	ssfn_dst.y += 17;
	ssfn_dst.x = 0;
}
