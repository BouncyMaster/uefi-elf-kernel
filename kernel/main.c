// Kernel entry point

#include <bool.h>
#include <int.h>
#include "kernel.h"

#define TEST_SCREEN_PRIMARY_COLOR       0x00FF40FF
#define TEST_SCREEN_SECONDARY_COLOR     0x00FFFFFF
// Whether to draw a test pattern to video output.
#define DRAW_TEST_SCREEN 1

void
draw_rect(uint32 * const framebufferPointer, const uint32 pixelsPerScanline)
{
	// Pointer to the current pixel in the buffer
	uint32 *at;
	uint16 col = 0;

	for (uint16 row = 0; row < 10; row++){
		for (col = 0; col < 10; col++){
			at = framebufferPointer + 400;
			at += (400 * pixelsPerScanline);

			*at = TEST_SCREEN_PRIMARY_COLOR;
		}
	}
}

void
kernel_main(Boot_Info *info)
{
	//draw_rect(info->videoModeInfo.framebufferPointer,
	//	info->videoModeInfo.pixelsPerScanline);

	uint32 *at;
	uint16 col = 0;

	for (uint16 row = 0; row < 10; row++){
		for (col = 0; col < 10; col++){
			at = info->video.framebuffer + 400;
			at += (400 * info->video.xRes);

			*at = TEST_SCREEN_PRIMARY_COLOR;
		}
	}

	for (;;);
}
