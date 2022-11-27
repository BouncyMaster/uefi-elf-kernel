/*
 * ORIGINALLY FROM:
 * @file graphics.c
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for working with the graphics protocol.
 * Contains functionality for initiating and working with the graphics protocol
 * and its associated framebuffers.
 */

#include "efilib.h"

#define TEST_COL_NUM		4
#define TEST_ROW_NUM		3
#define TEST_TOTAL_TILES	TEST_COL_NUM * TEST_ROW_NUM
#define TEST_PRIMARY_COLOR	0x00FF4000
#define TEST_SECONDARY_COLOR	0x00FF80BF

/*
 * @brief Finds and sets a video mode.
 * Finds and sets a particular video mode by its width, height and pixel format.
 * Tests all video modes copatible with the provided protocol, calling SetMode
 * when one is found.
 * @param[in]  protocol			The protocol to find the video mode in.
 * @param[in]  targetWidth		The target width to search for.
 * @param[in]  targetHeight		The target height to search for.
 * @param[in]  targetPixelFormat	The target pixel format to search for.
 */
static void
set_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL * const protocol,
	const UINT32 targetWidth, const UINT32 targetHeight,
	const EFI_GRAPHICS_PIXEL_FORMAT targetPixelFormat)
{
	EFI_STATUS status;
	UINTN size;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *modeInfo;

	for (UINT32 i = 0; i < protocol->Mode->MaxMode; i++){
		status = protocol->QueryMode(protocol, i, &size, &modeInfo);
		efi_assert(status, L"graphics:set_mode:QueryMode");

		if (modeInfo->HorizontalResolution == targetWidth &&
			modeInfo->VerticalResolution == targetHeight &&
			modeInfo->PixelFormat == targetPixelFormat) {

			status = protocol->SetMode(protocol, i);
			efi_assert(status, L"graphics:set_mode:SetMode");
			return;
		}
	}
#ifdef DEBUG
	err_handle(EFI_UNSUPPORTED, L"graphics:set_mode");
#endif
}

EFI_GRAPHICS_OUTPUT_PROTOCOL *
graphics_init(const UINT32 targetWidth, const UINT32 targetHeight,
	const EFI_GRAPHICS_PIXEL_FORMAT targetPixelFormat)
{
	EFI_STATUS status;
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *protocol;

	status = BOOT_SERVICES->LocateProtocol(&gopGuid, 0, (void **)&protocol);
	efi_assert(status, L"graphics:init");

	set_mode(protocol, targetWidth, targetHeight, targetPixelFormat);

	return protocol;
}

void
graphics_draw_rect(EFI_GRAPHICS_OUTPUT_PROTOCOL * const protocol,
	const UINT16 x, const UINT16 y, const UINT16 width, const UINT16 height,
	const UINT32 color)
{
	// Pointer to the current pixel in the buffer
	UINT32 *at;

	for (UINT16 row = 0; row < height; row++){
		for (UINT16 col = 0; col < width; col++){
			at = (UINT32 *)protocol->Mode->FrameBufferBase + x + col;
			at += ((y + row) * protocol->Mode->Info->PixelsPerScanLine);

			*at = color;
		}
	}
}

void
graphics_draw_test(EFI_GRAPHICS_OUTPUT_PROTOCOL * const protocol)
{
	const UINT16 tileWidth = protocol->Mode->Info->HorizontalResolution /
		TEST_COL_NUM;
	const UINT16 tileHeight = protocol->Mode->Info->VerticalResolution /
		TEST_ROW_NUM;

	UINT8 x, y;
	UINT32 color;

	for (UINT8 p = 0; p < TEST_TOTAL_TILES; p++){
		x = p % TEST_COL_NUM;
		y = p / TEST_COL_NUM;

		if (((y % 2) + x) % 2)
			color = TEST_SECONDARY_COLOR;
		else
			color = TEST_PRIMARY_COLOR;

		graphics_draw_rect(protocol, tileWidth * x, tileHeight * y,
			tileWidth, tileHeight, color);
	}
}
