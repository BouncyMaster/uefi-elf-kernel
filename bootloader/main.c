#include "efilib.h"
#include "graphics.h"
#include "fs.h"

#define TARGET_SCREEN_WIDTH	800
#define TARGET_SCREEN_HEIGHT	600
#define TARGET_PIXEL_FORMAT	PixelBlueGreenRedReserved8BitPerColor

/*
 * Whether to draw a test pattern to video output to test the graphics output
 * service.
 */
#define DRAW_TEST_SCREEN 1

EFI_STATUS
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fsp;
	EFI_FILE_PROTOCOL *root;

	// Sets global EFI table variables
	efi_init(ImageHandle, SystemTable);

	// Disable watchdog timer
	//status = BOOT_SERVICES->SetWatchdogTimer(0, 0, 0, 0);
	//efi_assert(status, L"main:SetWatchdogTimer");

	gop = graphics_init(TARGET_SCREEN_WIDTH, TARGET_SCREEN_HEIGHT,
		TARGET_PIXEL_FORMAT);

	#if DRAW_TEST_SCREEN != 0
		graphics_draw_test(gop);
	#endif

	fsp = fs_init();
	status = fsp->OpenVolume(fsp, &root);
	efi_assert(status, L"main:OpenVolume");

	// We use this for loop to hang.
	for(;;){};

	return EFI_SUCCESS;
}