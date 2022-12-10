#include "efilib.h"
#include "graphics.h"
#include "fs.h"
#include "loader.h"
#include "kernel.h"

#define TARGET_SCREEN_WIDTH	800
#define TARGET_SCREEN_HEIGHT	600
#define TARGET_PIXEL_FORMAT	PixelBlueGreenRedReserved8BitPerColor
/*
 * Whether to draw a test pattern to video output to test the graphics output
 * service.
 */
#define DRAW_TEST_SCREEN 0

#define KERNEL_EXECUTABLE_PATH L"\\kernel.elf"

static void
get_memory_map(EFI_MEMORY_DESCRIPTOR **memoryMap, UINTN *memoryMapSize,
	UINTN *memoryMapKey, UINTN *descriptorSize, UINT32 *descriptorVersion)
{
	EFI_STATUS status;

	status = BOOT_SERVICES->GetMemoryMap(memoryMapSize, *memoryMap,
		memoryMapKey, descriptorSize, descriptorVersion);
	/*
	 * This will always fail on the first attempt, this call will return the
	 * required buffer size.
	 */
#ifdef DEBUG
	if (EFI_ERROR(status) && status != EFI_BUFFER_TOO_SMALL){
		err_handle(status, L"main:get_memory_map:1");
	}
#endif

	/*
	 * According to: https://stackoverflow.com/a/39674958/5931673
	 * Up to two new descriptors may be created in the process of allocating
	 * the new pool memory.
	 */
	*memoryMapSize += 2 * (*descriptorSize);

	status = BOOT_SERVICES->AllocatePool(EfiLoaderData, *memoryMapSize,
		memoryMap);
	efi_assert(status, L"main:get_memory_map:AllocatePool");

	status = BOOT_SERVICES->GetMemoryMap(memoryMapSize, *memoryMap,
		memoryMapKey, descriptorSize, descriptorVersion);
	efi_assert(status, L"main:get_memory_map:2");
}

EFI_STATUS
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fsp;

	EFI_MEMORY_DESCRIPTOR *memoryMap;
	UINTN	memoryMapSize, memoryMapKey, descriptorSize;
	UINT32	descriptorVersion;

	EFI_FILE_PROTOCOL *root;
	EFI_PHYSICAL_ADDRESS kernelEntryPoint;
	// Function pointer to the kernel entry point
	void (*kernel_entry)(Kernel_Info* bootInfo);
	// Passed to the kernel
	Kernel_Info info;

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

	kernelEntryPoint = load_kernel(root, KERNEL_EXECUTABLE_PATH);

	// Set kernel video info
	boot.videoModeInfo.framebufferPointer = gop->Mode->FrameBufferBase;
	boot.videoModeInfo.horizontalRes = gop->Mode->Info->HorizontalResolution;
	boot.videoModeInfo.verticalRes = gop->Mode->Info->VerticalResolution;
	boot.videoModeInfo.pixelsPerScanline = gop->Mode->Info->PixelsPerScanLine;

	get_memory_map(&memoryMap, &memoryMapSize, &memoryMapKey,
		&descriptorSize, &descriptorVersion);

	status = BOOT_SERVICES->ExitBootServices(ImageHandle, memoryMapKey);
	efi_assert(status, L"main:ExitBootServices");

	// Set kernel info
	boot.memoryMap = memoryMap;
	boot.memoryMapSize = memoryMapSize;
	boot.memoryMapDescriptorSize = descriptorSize;

	// Cast pointer to kernel entry.
	kernel_entry = (void (*)(Kernel_Info *))*kernelEntryPoint;
	// Jump to kernel entry.
	kernel_entry(&info);

	// Return an error if this code is ever reached.
	return LOAD_ERROR;
}