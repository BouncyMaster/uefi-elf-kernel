// Kernel entry point

#include <bool.h>
#include <int.h>

#include "boot.h"
#include "print.h"
#include "memory.h"

// Needed to map code memory
extern u64 kernel_start;
extern u64 kernel_end;

void
kernel_main(Boot_Info *info)
{
	u32 *at;
	u16 col = 0;

	// Place a pixel at the center of the screen
	for (u16 row = 0; row < 10; row++){
		for (col = 0; col < 10; col++){
			at = info->video.framebuffer + 400;
			at += (400 * info->video.xRes);

			*at = 0xFFFF00;
		}
	}

	u64 kernelPages = ((u64)&kernel_end - (u64)&kernel_start) / 4096 + 1;

	print_init(&info->video);

	memory_init(info->memoryMap, info->memoryMapSize,
		info->memoryMapDescSize);
	memory_lock_pages(&kernel_start, kernelPages);

	print("Free: ");
	print(u_to_str(freeMemory / 1024));
	print(" KB");
	print_lf();

	print("Used: ");
	print(u_to_str(usedMemory / 1024));
	print(" KB");
	print_lf();

	print("Reserved: ");
	print(u_to_str(reservedMemory / 1024));
	print(" KB");
	print_lf();

	for (u8 i = 0; i < 5; i++){
		void *address = memory_request_page();
		print(u_to_str((u64)address));
		print_lf();
	}

	print("Used: ");
	print(u_to_str(usedMemory / 1024));
	print(" KB");
	print_lf();

	for (;;);
}