// Kernel entry point

#include <bool.h>
#include <int.h>

#include "boot.h"
#include "print.h"
#include "memory.h"

void
kernel_main(Boot_Info *info)
{
	u32 *at;
	u16 col = 0;

	for (u16 row = 0; row < 10; row++){
		for (col = 0; col < 10; col++){
			at = info->video.framebuffer + 400;
			at += (400 * info->video.xRes);

			*at = 0xFFFF00;
		}
	}

	print_init(&info->video);
	print("Size: ");
	print(u_to_str(memory_get_size(info->memoryMap, info->memoryMapSize,
		info->memoryMapDescriptorSize)));
	print(" Bytes");

	for (;;);
}