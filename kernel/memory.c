/*
 * 1 page = 4 KB = 4096 B
 * Memory page management using a bitmap: 1 bit = 1 page
 * EFI Returns memory descriptor size in pages, not bytes.
 */

#include <int.h>
#include "boot.h"
#include "bitmap.h"

/*
 * TODO: maybe move this to a new file for handling buffers
 * TODO2: if we are guaranteed that the buffer size is divisible by 8 bytes,
 * then maybe we can cast to u64 instead for performance;
 * Not 100% sure it will be faster.
 */
// Zero out the buffer
static void
buffer_zero(void *buffer, u64 size)
{
	for (u64 i = 0; i < size; i++){
		*(u8 *)(buffer + i) = 0;
	}
}
