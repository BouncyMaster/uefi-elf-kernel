// Kernel entry point

#include <bool.h>
#include <int.h>
#include "kernel.h"

#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include "ssfn.h"

extern ssfn_font_t _binary_font_sfn_start;

static void
puts(char *s)
{
	while (*s)
		ssfn_putc(*s++);
}

void
kernel_main(Boot_Info *info)
{
	u32 *at;
	u16 col = 0;

	ssfn_src = &_binary_font_sfn_start;

	ssfn_dst.ptr = (u8 *)info->video.framebuffer;
	ssfn_dst.w = info->video.xRes;
	ssfn_dst.h = info->video.yRes;
	ssfn_dst.p = info->video.xRes * 4; // 4 bytes per pixel
	ssfn_dst.x = ssfn_dst.y = 0;
	ssfn_dst.fg = 0xFFFFFF;

	for (u16 row = 0; row < 10; row++){
		for (col = 0; col < 10; col++){
			at = info->video.framebuffer + 400;
			at += (400 * info->video.xRes);

			*at = 0xFFFF00;
		}
	}

	puts("Hello");

	for (;;);
}