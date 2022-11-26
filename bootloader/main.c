#include "efilib.h"
#include "graphics.h"

#define TARGET_SCREEN_WIDTH     800
#define TARGET_SCREEN_HEIGHT    600
#define TARGET_PIXEL_FORMAT     PixelBlueGreenRedReserved8BitPerColor

/**
 * Whether to draw a test pattern to video output to test the graphics output
 * service.
 */
#define DRAW_TEST_SCREEN 1

EFI_STATUS
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS status;

	// Sets global EFI table variables
	efi_init(ImageHandle, SystemTable);

	// Disable watchdog timer
	status = BOOT_SERVICES->SetWatchdogTimer(0, 0, 0, 0);
	if (EFI_ERROR(status))
		err_handle(status, L"main:SetWatchdogTimer");

	// Graphics
	//struct Graphics graphics;
	//graphics_init(&graphics);

	// Open the graphics output protocol from the handle for the active console
	// output device and use it to draw the boot screen.
	// The console out handle exposed by the System Table is documented in the
	// UEFI Spec page 92.

	// This should be moved to graphics.c
	// TODO: figure out why osdev wiki uses LocateProtocol instead of
	// OpenProtocol here.
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

	status = BOOT_SERVICES->LocateProtocol(&gopGuid, 0, (void **)&gop);
	if (EFI_ERROR(status))
		err_handle(status, L"main:LocateProtocol");

	if (gop){
		graphics_set_mode(gop, TARGET_SCREEN_WIDTH,
			TARGET_SCREEN_HEIGHT, TARGET_PIXEL_FORMAT);

		#if DRAW_TEST_SCREEN != 0
			graphics_draw_test(gop);
		#endif
	}

	err_handle(EFI_UNSUPPORTED, L"testing");
	// We use this for loop to hang.
	for(;;){};

	return EFI_SUCCESS;
}