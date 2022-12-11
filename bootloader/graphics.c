#include "efilib.h"

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
