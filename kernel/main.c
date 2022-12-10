// Kernel entry point

#include <bool.h>
#include <int.h>
#include "kernel.h"
#include "graphics.h"

#define TEST_SCREEN_COL_NUM             4
#define TEST_SCREEN_ROW_NUM             3
#define TEST_SCREEN_TOTAL_TILES         TEST_SCREEN_COL_NUM * TEST_SCREEN_ROW_NUM
#define TEST_SCREEN_PRIMARY_COLOR       0x00FF40FF
#define TEST_SCREEN_SECONDARY_COLOR     0x00FF00CF
// Whether to draw a test pattern to video output.
#define DRAW_TEST_SCREEN 1

static void
draw_test_screen(Boot_Info *info)
{
	const uint16 tileWidth = info->videoModeInfo.horizontalRes /
		TEST_SCREEN_COL_NUM;
	const uint16 tileHeight = info->videoModeInfo.verticalRes /
		TEST_SCREEN_ROW_NUM;

	uint32 color;

	for (uint8 p = 0; p < TEST_SCREEN_TOTAL_TILES; p++){
		uint8 x = p % TEST_SCREEN_COL_NUM;
		uint8 y = p / TEST_SCREEN_COL_NUM;

		if (((y % 2) + x) % 2)
			color = TEST_SCREEN_SECONDARY_COLOR;
		else
			color = TEST_SCREEN_PRIMARY_COLOR;

		draw_rect(info->videoModeInfo.framebufferPointer,
			info->videoModeInfo.pixelsPerScanline,
			tileWidth * x, tileHeight * y,
			tileWidth, tileHeight, color);
	}
}

void
kernel_main(Boot_Info *info)
{
	#if DRAW_TEST_SCREEN
		draw_test_screen(info);
	#endif
}
