// Kernel entry point

#include <bool.h>
#include <int.h>
#include "boot.h"
#include "print.h"

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
	print("Hello ");
	print(to_str(1234));

	for (;;);
}