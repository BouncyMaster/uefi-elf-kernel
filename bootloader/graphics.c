/*
 * ORIGINALLY FROM:
 * @file graphics.c
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for working with the graphics protocol.
 * Contains functionality for initiating and working with the graphics protocol
 * and its associated framebuffers.
 */

#include <efi.h>
#include "graphics.h"
#include "err_handle.h"

#define TEST_SCREEN_COL_NUM             4
#define TEST_SCREEN_ROW_NUM             3
#define TEST_SCREEN_TOTAL_TILES         TEST_SCREEN_COL_NUM * TEST_SCREEN_ROW_NUM
#define TEST_SCREEN_PRIMARY_COLOUR      0x00FF4000
#define TEST_SCREEN_SECONDARY_COLOUR    0x00FF80BF

#define ERR_FIND_MODE L"graphics:find_mode()\r\n"

/*
 * @brief Finds a video mode.
 * Finds a particular video mode by its width, height and pixel format.
 * Tests all video modes copatible with the provided protocol, populating the
 * `video_mode` variable on success.
 * @param[in]  protocol			The protocol to find the video mode in.
 * @param[in]  targetWidth		The target width to search for.
 * @param[in]  targetHeight		The target height to search for.
 * @param[in]  targetPixelFormat	The target pixel format to search for.
 *
 * @retval videoMode.
 */
static UINT32
find_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL * const protocol,
	const UINT32 targetWidth, const UINT32 targetHeight,
	const EFI_GRAPHICS_PIXEL_FORMAT targetPixelFormat)
{
	EFI_STATUS status;
	UINTN size;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *modeInfo;

	for (UINT32 i = 0; i < protocol->MaxMode; i++){
		status = protocol->QueryMode(protocol, i, &size, &modeInfo);
		if (EFI_ERROR(status))
			err_handle(status, ERR_FIND_MODE);

		if (modeInfo->HorizontalResolution == targetWidth &&
			modeInfo->VerticalResolution == targetHeight &&
			modeInfo->PixelFormat == targetPixelFormat)
				return i;
	}
	err_handle(EFI_UNSUPPORTED, ERR_FIND_MODE);
}
